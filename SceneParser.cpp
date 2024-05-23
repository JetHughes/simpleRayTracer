#include "SceneParser.h"
#include "utility.h"

#include "Camera.h"
#include "PinholeCamera.h"

#include "LightSource.h"
#include "AmbientLightSource.h"
#include "DirectionalLightSource.h"
#include "PointLightSource.h"
#include "SpotLightSource.h"

#include "Material.h"

#include "Object.h"
#include "CSG.h"
#include "Cylinder.h"
#include "Cube.h"
#include "ObjectGroup.h"
#include "Plane.h"
#include "Sphere.h"
#include "Torus.h"

#include <iostream>
#include <fstream>
#include <sstream>



void SceneParser::read(const std::string& filename) {

	std::cout << "Reading scene from " << filename << std::endl;

	std::ifstream fin(filename);
	std::queue<Token> tokenStream;

	std::string line = "";
	int lineNumber = 0;
	while (std::getline(fin, line)) {
		++lineNumber;
		std::stringstream strstream(line);
		std::string tokenStr;
		while (strstream >> tokenStr) {
			if (tokenStr[0] == '#') {
				// A comment, skip te rest of this line
				break;
			} else {
				Token token;
				token.val = tokenStr;
				token.line = lineNumber;
				tokenStream.push(token);
			}
		}
	}

	while (!tokenStream.empty()) {
		Token token = tokenStream.front();
		tokenStream.pop();

		if (token == "SCENE") {
			parseSceneBlock(tokenStream);
		} else if (token == "CAMERA") {
			parseCameraBlock(tokenStream);
		} else if (token == "LIGHT") {
			scene->addLight(parseLightBlock(tokenStream));
		} else if (token == "OBJECT") {
			scene->addObject(parseObjectBlock(tokenStream));
		} else if (token == "MATERIAL") {
			parseMaterialBlock(tokenStream);
		} else if (token == "OBJECTGROUP") {
			scene->addObject(parseObjectGroupBlock(tokenStream));
		} else if (token == "CSG") {
			scene->addObject(parseCSGBlock(tokenStream));
		} else {
			std::cerr << "Unexpected token " << token.val << " on line " << token.line << std::endl;
			std::cerr << "Expected a block token - SCENE, CAMERA, LIGHT, OBJECT, or MATERIAL" << std::endl;
			exit(-1);
		}

	}

	fin.close();
}

std::string SceneParser::parseString(std::queue<Token>& tokens) {
	std::string result = tokens.front().val;
	tokens.pop();
	return result;
}

double SceneParser::parseNumber(std::queue<Token>& tokens) {
	Token t = tokens.front();
	tokens.pop();

	char* endPtr;
	double result = strtod(t.val.c_str(), &endPtr);
	if (endPtr != t.val.c_str() + t.val.length()) {
		std::cerr << "Expected a number but found " << t.val << " on line " << t.line << std::endl;
		exit(-1);
	}

	return result;
}

Colour SceneParser::parseColour(std::queue<Token>& tokens) {
	Colour result;
	result.red = parseNumber(tokens);
	result.green = parseNumber(tokens);
	result.blue = parseNumber(tokens);
	return result;
}

void SceneParser::parseSceneBlock(std::queue<Token>& tokens) {
	while (tokens.front() != "END") {
		Token t = tokens.front();
		tokens.pop();
		if (t == "BACKGROUNDCOLOUR") {
			scene->backgroundColour = parseColour(tokens);
		} else if (t == "RENDERSIZE") {
			scene->renderWidth = int(parseNumber(tokens));
			scene->renderHeight = int(parseNumber(tokens));
		} else if (t == "FILENAME") {
			scene->filename = parseString(tokens);
			tokens.pop();
		}
	}
	// Pop the END token
	tokens.pop();
}

void SceneParser::parseCameraBlock(std::queue<Token>& tokens) {
	Token cameraType = tokens.front();
	tokens.pop();

	std::shared_ptr<Camera> camera;
	if (cameraType == "PINHOLECAMERA") {
		double focalLength = parseNumber(tokens);
		camera = std::shared_ptr<PinholeCamera>(new PinholeCamera(focalLength));
		scene->setCamera(camera);
	} else {
		std::cerr << "Unexpected camera type " << cameraType.val << " on line " << cameraType.line << std::endl;
		exit(-1);
	}

	// Parse camera details
	while (tokens.front() != "END") {
		Token t = tokens.front();
		tokens.pop();

		if (t == "ROTATE") {
			std::string axis = parseString(tokens);
			double angle = parseNumber(tokens);
			if (axis == "X") {
				camera->transform.rotateX(angle);
			} else if (axis == "Y") {
				camera->transform.rotateY(angle);
			} else if (axis == "Z") {
				camera->transform.rotateZ(angle);
			}
		} else if (t == "TRANSLATE") {
			double tx = parseNumber(tokens);
			double ty = parseNumber(tokens);
			double tz = parseNumber(tokens);
			camera->transform.translate(tx, ty, tz);
		} else if (t == "SCALE") {
			double s = parseNumber(tokens);
			camera->transform.scale(s);
		} else if (t == "SCALE3") {
			double sx = parseNumber(tokens);
			double sy = parseNumber(tokens);
			double sz = parseNumber(tokens);
			camera->transform.scale(sx, sy, sz);
		} else {
			std::cerr << "Unexpected token '" << t.val << "' on line " << t.line << std::endl;
			exit(-1);
		}
	}
	// Pop the last End
	tokens.pop();

}


std::shared_ptr<LightSource> SceneParser::parseLightBlock(std::queue<Token>& tokens) {
	Token lightType = tokens.front();
	tokens.pop();

	Point loc;
	Colour col;
	Direction dir;
	double angle = 0;

	while (tokens.front() != "END") {
		Token t = tokens.front();
		tokens.pop();
		if (t == "LOCATION") {
			loc(0) = parseNumber(tokens);
			loc(1) = parseNumber(tokens);
			loc(2) = parseNumber(tokens);
		} else if (t == "COLOUR") {
			col = parseColour(tokens);
		} else if (t == "DIRECTION") {
			dir(0) = parseNumber(tokens);
			dir(1) = parseNumber(tokens);
			dir(2) = parseNumber(tokens);
		} else if (t == "ANGLE") {
			angle = parseNumber(tokens);
		} else {
			std::cerr << "Unexpected token '" << t.val << "' on line " << t.line << std::endl;
			exit(-1);
		}
	}
	// Pop the END token
	tokens.pop();

	std::shared_ptr<LightSource> light;
	if (lightType == "POINTLIGHT") {
		light = std::shared_ptr<PointLightSource>(new PointLightSource(col, loc));
	} else if (lightType == "AMBIENTLIGHT") {
		light = std::shared_ptr<AmbientLightSource>(new AmbientLightSource(col));
	} else if (lightType == "DIRECTIONALLIGHT") {
		light = std::shared_ptr<DirectionalLightSource>(new DirectionalLightSource(col, dir));
	} else if (lightType == "SPOTLIGHT") {
		light = std::shared_ptr<SpotLightSource>(new SpotLightSource(col, loc, dir, angle));
	} else {
		std::cerr << "Unexpected light type '" << lightType.val << "' on line " << lightType.line << std::endl;
		exit(-1);
	}

	return light;
}

std::shared_ptr<Object> SceneParser::parseObjectBlock(std::queue<Token>& tokens) {
	Token objectType = tokens.front();
	tokens.pop();
	std::shared_ptr<Object> object;
	if (objectType == "SPHERE") {
		object = std::shared_ptr<Sphere>(new Sphere());
	} else if (objectType == "CUBE") {
		object = std::shared_ptr<Cube>(new Cube());
	} else if (objectType == "PLANE") {
		object = std::shared_ptr<Plane>(new Plane());
	} else if (objectType == "CYLINDER") {
		object = std::shared_ptr<Cylinder>(new Cylinder());
	} else if (objectType == "TORUS") {
		double radius = parseNumber(tokens);
		object = std::shared_ptr<Torus>(new Torus(radius));
	} else {
		std::cerr << "Unexpected object type '" << objectType.val << "' on line " << objectType.line << std::endl;
		exit(-1);
	}
	// Parse object details
	while (tokens.front() != "END") {
		Token t = tokens.front();
		tokens.pop();

		if (t == "ROTATE") {
			std::string axis = parseString(tokens);
			double angle = parseNumber(tokens);
			if (axis == "X") {
				object->transform.rotateX(angle);
			} else if (axis == "Y") {
				object->transform.rotateY(angle);
			} else if (axis == "Z") {
				object->transform.rotateZ(angle);
			}
		} else if (t == "TRANSLATE") {
			double tx = parseNumber(tokens);
			double ty = parseNumber(tokens);
			double tz = parseNumber(tokens);
			object->transform.translate(tx, ty, tz);
		} else if (t == "SCALE") {
			double s = parseNumber(tokens);
			object->transform.scale(s);
		} else if (t == "SCALE3") {
			double sx = parseNumber(tokens);
			double sy = parseNumber(tokens);
			double sz = parseNumber(tokens);
			object->transform.scale(sx, sy, sz);
		} else if (t == "MATERIAL") {
			Token materialName = tokens.front();
			tokens.pop();
			auto material = materials.find(materialName.val);
			if (material == materials.end()) {
				std::cerr << "Undefined material '" << materialName.val << "' on line " << materialName.line << std::endl;
				exit(1);
			} else {
				object->material = material->second;
			}
		} else if (t == "COLOUR") {
			Colour objColour = parseColour(tokens);
			object->material.ambientColour = objColour;
			object->material.diffuseColour = objColour;
		} else if (t == "AMBIENT") {
			object->material.ambientColour = parseColour(tokens);
		} else if (t == "DIFFUSE") {
			object->material.diffuseColour = parseColour(tokens);
		} else if (t == "SPECULAR") {
			object->material.specularColour = parseColour(tokens);
			object->material.specularExponent = parseNumber(tokens);
		} else if (t == "MIRROR") {
			object->material.mirrorColour = parseColour(tokens);
		} else {
			std::cerr << "Unexpected token '" << t.val << "' on line " << t.line << std::endl;
			exit(-1);
		}

	}
	// Pop the final END
	tokens.pop();
	return object;

}

void SceneParser::parseMaterialBlock(std::queue<Token>& tokens) {
	Token materialName = tokens.front();
	tokens.pop();
	if (materials.find(materialName.val) == materials.end()) {
		materials[materialName.val] = Material();
	} else {
		std::cerr << "Warning: duplicate definition of material " << materialName.val << " on line " << materialName.line << std::endl;
		exit(-1);
	}

	Material& material = materials.find(materialName.val)->second;

	while (tokens.front() != "END") {
		Token token = tokens.front();
		tokens.pop();
		if (token == "COLOUR") {
			Colour objColour = parseColour(tokens);
			material.ambientColour = objColour;
			material.diffuseColour = objColour;
		} else if (token == "AMBIENT") {
			material.ambientColour = parseColour(tokens);
		} else if (token == "DIFFUSE") {
			material.diffuseColour = parseColour(tokens);
		} else if (token == "SPECULAR") {
			material.specularColour = parseColour(tokens);
			material.specularExponent = parseNumber(tokens);
		} else if (token == "MIRROR") {
			material.mirrorColour = parseColour(tokens);
		} else {
			std::cerr << "Unexpected token '" << token.val << "' on line " << token.line << std::endl;
			exit(-1);
		}
	}
	// Pop the END token
	tokens.pop();
}
std::shared_ptr<Object> SceneParser::parseObjectGroupBlock(std::queue<Token>& tokens) {
	std::shared_ptr<ObjectGroup> object(new ObjectGroup());
	
	while (tokens.front() != "END") {
		Token t = tokens.front();
		tokens.pop();
		if (t == "OBJECT") {
			object->addObject(parseObjectBlock(tokens));
		} else if (t == "ROTATE") {
			std::string axis = parseString(tokens);
			double angle = parseNumber(tokens);
			if (axis == "X") {
				object->transform.rotateX(angle);
			} else if (axis == "Y") {
				object->transform.rotateY(angle);
			} else if (axis == "Z") {
				object->transform.rotateZ(angle);
			}
		} else if (t == "TRANSLATE") {
			double tx = parseNumber(tokens);
			double ty = parseNumber(tokens);
			double tz = parseNumber(tokens);
			object->transform.translate(tx, ty, tz);
		} else if (t == "SCALE") {
			double s = parseNumber(tokens);
			object->transform.scale(s);
		} else if (t == "SCALE3") {
			double sx = parseNumber(tokens);
			double sy = parseNumber(tokens);
			double sz = parseNumber(tokens);
			object->transform.scale(sx, sy, sz);
		} else {
			std::cerr << "Unexpected token '" << t.val << "' on line " << t.line << std::endl;
			exit(-1);
		}
	}
	// Pop END token
	tokens.pop();

	return object;
}

std::shared_ptr<Object> SceneParser::parseCSGBlock(std::queue<Token>& tokens) {

	std::shared_ptr<CSG> object;

	Token op = tokens.front();
	tokens.pop();
	if (op == "UNION") {
		object = std::shared_ptr<CSG>(new CSG(CSG::UNION));
	} else if (op == "INTERSECTION") {
		object = std::shared_ptr<CSG>(new CSG(CSG::INTERSECTION));
	} else if (op == "DIFFERENCE") {
		object = std::shared_ptr<CSG>(new CSG(CSG::DIFFERENCE));
	}
	int objCount = 0;

	while (tokens.front() != "END") {
		Token t = tokens.front();
		tokens.pop();
		if (t == "OBJECT") {
			if (objCount == 0) {
				object->setLeft(parseObjectBlock(tokens));
			} else if (objCount == 1) {
				object->setRight(parseObjectBlock(tokens));
			} else {
				std::cerr << "Too many objects in CSG operation on line " << t.line << std::endl;
				exit(1);
			}
			++objCount;
		} else if (t == "ROTATE") {
			std::string axis = parseString(tokens);
			double angle = parseNumber(tokens);
			if (axis == "X") {
				object->transform.rotateX(angle);
			} else if (axis == "Y") {
				object->transform.rotateY(angle);
			} else if (axis == "Z") {
				object->transform.rotateZ(angle);
			}
		} else if (t == "TRANSLATE") {
			double tx = parseNumber(tokens);
			double ty = parseNumber(tokens);
			double tz = parseNumber(tokens);
			object->transform.translate(tx, ty, tz);
		} else if (t == "SCALE") {
			double s = parseNumber(tokens);
			object->transform.scale(s);
		} else if (t == "SCALE3") {
			double sx = parseNumber(tokens);
			double sy = parseNumber(tokens);
			double sz = parseNumber(tokens);
			object->transform.scale(sx, sy, sz);
		} else {
			std::cerr << "Unexpected token '" << t.val << "' on line " << t.line << std::endl;
			exit(-1);
		}
	}
	// Pop END token
	tokens.pop();

	if (objCount != 2) {
		std::cerr << "Unexpected exaclty two objects in CSG operation on line " << op.line << std::endl;
		exit(-1);
	}
	return object;
}
