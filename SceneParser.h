#pragma once

#ifndef SCENE_PARSER_H
#define SCENE_PARSER_H

#include "NonCopyable.h"

#include "Colour.h"
#include "Material.h"
#include "Scene.h"
#include "LightSource.h"

#include "utility.h"

#include <string>
#include <vector>
#include <queue>
#include <map>

/** \file
 * \brief SceneParser header file.
 */

/** \brief A single token (word) 
 *
 * When parsing the input file, it is broken into tokens.
 * These are essentially std::strings, but allow for case-insensitive
 * equality comparison, and remember which line they were on for error reporting.
 */
struct Token {
	std::string val = ""; //!< The value of the Token.
	int line = 0; //!< The line on which the Token appeared.

	/** Equality operator */
	bool operator==(const std::string& str) {
		return toUpper(val) == toUpper(str);
	}

	/** Inequality operator */
	bool operator!=(const std::string& str) {
		return !operator==(str);
	}
};

/**
 * \brief Simple parser for Scene description files.
 *
 * This class implements a simple parser for Scene description files.
 * A SceneReader is linked to a Scene object, and can then read one or more
 * text files describing Scene properties, Cameras, LightSources, and Objects.
 * These files are defined in blocks, and are case-insensitive.
 * Whitespace is contracted, so new lines, spaces, and tabs are all just token separators.
 * Comments are introduced with \c #, and continue to the end of the line.
 *
 * There are five main types of block:
 * - Scene blocks
 * - Camera blocks
 * - Light blocks
 * - Material blocks
 * - Object blocks
 * Each block begins with a keyword (the type of block) and ends with the token 'End'
 * Details of each block type are given below.
 *
 * <b>Scene Blocks</b>
 *
 * Example:
\verbatim
 Scene
   renderSize 800 600
   backgroundColour 0.5 0.5 0.5
   filename output.png
   rayDepth 5
 End
 \endverbatim
 *
 * Allowed elements within a Scene block are:
 * - <tt>renderSize [width] [height]</tt>: Set the Scene's \c renderWidth and \c renderHeight properties to the given values.
 * - <tt>backgroundColour [red] [green] [blue]</tt>: Set the Scene's \c backgroundColour property to the given Colour.
 * - <tt>filename [file]</tt>: Set the Scene's \c filename property to the given value.
 * - <tt>rayDepth [number]</tt>: Set the Scene's \c rayDepth property to the given value.
 *
 * <b>Camera Blocks</b>
 *
 * Example:
\verbatim
Camera PinholeCamera 1.5
  Rotate X -15
  Translate 0 -8 -15
End
\endverbatim
 *
 * A Camera block starts with a line giving the type of Camera and a parameter for that Camera if one can be provided.
 * In the example, a PinholeCamera is created with a focalLength of 1.5
 * Allowed elements within a Scene block are:
 * - <tt>Rotate [axis] [angle]</tt>: Apply a rotation of angle degrees about the specified axis (one of X, Y, or Z) to the Camera.
 * - <tt>Translate [x] [y] [z]</tt>: Apply the specified translation to the Camera.
 * - <tt>Scale [s]</tt>: Apply a uniform scaling by the given value to the Camera.
 * - <tt>Scale3 [sx] [sy] [sz]</tt>: Apply different scaling factors along the three axes to the Camera.
 *
 * <b>Light Blocks</b>
 *
 * Examples:
\verbatim
Light PointLight
  Location 1 2 3
  Colour 20 30 40
End

Light SpotLight
  Location 0 -5 0
  Colour 10 10 10
  Direction 0 1 0
  Angle 30
End
\endverbatim
 *
 * A Light block starts with a line giving the type of Light.
 *
 * Allowed elements within a Light block are:
 * - <tt>Colour [red] [green] [blue]</tt>: Set the Light's \c colour property to the given Colour.
 * - <tt>Location [x] [y] [z]</tt>: Set the Light's \c location property to the given co-ordinates.
 * - <tt>Direction [x] [y] [z]</tt>: Set the Light's \c location property to the given co-ordinates.
 * - <tt>Angle [x]</tt>
 *
 * Not all light source types use all parameters, and any surplus elements are ignored:
 * - <tt>AmbientLight</tt>s use <tt>Colour</tt> only
 * - <tt>PointLight</tt>s use <tt>Colour</tt> and <tt>Location</tt>
 * - <tt>DirectionalLight</tt>s use <tt>Colour</tt> and <tt>Direction</tt>
 * - <tt>SpotLight</tt>s use <tt>Colour</tt>, <tt>Location</tt>, <tt>Direction</tt>, and <tt>Angle</tt>
 *
 * <b> Material Blocks </b>
 *
 * Example:
 *
\verbatim
Material Gold
  Colour 1 0.88 0.25
  Specular 1 1 1 100
  Mirror 1 0.88 0.25
End
\endverbatim
 *
 * A Material block starts with a line giving the name of a Material (in this example, 'Gold').
 * Allowed elements within a Material block are:
 * - <tt>Ambient [red] [green] [blue]</tt>: Set the Material's \c ambientColour property to the given Colour.
 * - <tt>Diffuse [red] [green] [blue]</tt>: Set the Material's \c diffuseColour property to the given Colour.
 * - <tt>Colour [red] [green] [blue]</tt>: Set the Material's \c ambientColour and \c diffuseColour properties to the given Colour.
 * - <tt>Specular [red] [green] [blue] [exponent]</tt>: Set the Material's \c specularColour property to the given Colour, and its \c specularExponent to the given value.
 * - <tt>Mirror [red] [green] [blue]</tt>: Set the Material's \c diffuseColour property to the given Colour.
 *
 * <b> Object Blocks </b>
 *
 * Example:
\verbatim
Object Sphere
  Material Gold
  Specular 0.7 0.7 0.7 75
  Scale3 1 2 3
  Rotate Y 45
  Translate 0 0 3
\endverbatim
 *
 * An Object block starts with a line giving the type of Object to create (a Sphere in this case).
 * Allowed elements within an Object block are:
 * - <tt>Rotate [axis] [angle]</tt>: Apply a rotation of angle degrees about the specified axis (one of X, Y, or Z) to the Object.
 * - <tt>Translate [x] [y] [z]</tt>: Apply the specified translation to the Object.
 * - <tt>Scale [s]</tt>: Apply a uniform scaling by the given value to the Object.
 * - <tt>Scale3 [sx] [sy] [sz]</tt>: Apply different scaling factors along the three axes to the Object.
 * - <tt>Material [name]</tt>: Copy the properties of a previously defined Material to the Object.
 * - <tt>Ambient [red] [green] [blue]</tt>: Set the \c ambientColour property of the Object's Material to the given Colour.
 * - <tt>Diffuse [red] [green] [blue]</tt>: Set the \c diffuseColour property of the Object's Material to the given Colour.
 * - <tt>Colour [red] [green] [blue]</tt>: Set the \c ambientColour and \c diffuseColour properties of the Object's Material to the given Colour.
 * - <tt>Specular [red] [green] [blue] [exponent]</tt>: Set the\c specularColour property to the given Colour, and its \c specularExponent to the given value.
 * - <tt>Mirror [red] [green] [blue]</tt>: Set the \c diffuseColour property of the Object's Material to the given Colour.
 * Object types that can be read are: Cube, Cylinder, Plane, Sphere, and Torus
 *
 * Note that a Torus has a thickness, which is provided as an additional parameter, so a Torus with a thickness of 0.33 would be
 \verbatim
 Object Torus 0.33
	Colour 0 1.0 0
	Translate 0 0 3
 End
 \endverbatim
 *
 * There are also two types of compound Object blocks - ObjectGroup and CSG.
 * 
 * An ObjectGroup contains any number of Objects, and may have transform (Rotate, Translate, Scale) applied to it.
 * Example:
\verbatim
ObjectGroup

	Object Cube 
		Colour 1.0 0.0 0.0
		Translate 0 0 -1
	End

	Object Sphere 
		Colour 0.0 0.5 1.0
		Translate 0 0 1
	End

	Rotate X 90 # Note - applied to both the Cube and Sphere

End
\endverbatim
 *
 * CSG operations are formed from exactly two Objects and always begin with an operation - one of Union, Intersection, or Difference.
 * 
 * Example:
 \verbatim
 CSG Union
    
	Rotate X 90 # Note - applied to both the Cube and Sphere

    Object Cube 
		Colour 1.0 0.0 0.0
		Translate 0 0 -1
	End

	Object Sphere 
		Colour 0.0 0.5 1.0
		Translate 0 0 1
	End

 End
 \endverbatim
 * 
 * You cannot alter any Material properties (Colour, Mirror, Diffuse, etc.) in compound Object blocks.
 */
class SceneParser : private NonCopyable {

public:
	/** \brief SceneParser constructor.
	 *
	 * \param scene A pointer to the Scene which data will be read into.
	 */
	SceneParser(Scene* scene) : scene(scene) {}
	
	/** \brief SceneParser destructor. */
	~SceneParser() = default;

	/** \brief Read Scene data from a file.
	 *
	 * This method reads data from a file in the format discussed above.
	 * It adds information to the Scene linked to this SceneReader, and so
	 * multiple files can be combined into one Scene.
	 *
	 * If an error is encountered parsing the file, the program is terminated.
	 *
	 * \param filename The name of the file to read.
	 */
	void read(const std::string& filename);

private:

	/**
	 * \brief Pop the next Token and return it as a string. 
	 *
	 * \param tokens The queue of unparsed Tokens
	 * \return The next Token as a string.
	 */
	std::string parseString(std::queue<Token>& tokens);

	/**
	 * \brief Pop the next Token and return it as a number.
	 *
	 * \param tokens The queue of unparsed Tokens
	 * \return The next Token as a number.
	 */
	double parseNumber(std::queue<Token>& tokens);

	/** \brief Read a number information from a block of tokens.
	 *
	 * This tries to interpret the next item in a block of tokens as
	 * a number. The token is removed from the block.
	 *
	 * If there is a problem in this process, the program is terminated.
	 *
	 * \param tokens The queue of unparsed Tokens to read the Colour from.
	 * \return The Colour read from the block of tokens..
	 */
	Colour parseColour(std::queue<Token>& tokens);

	/** \brief Parse a block of tokens representing a Scene.
	 *
	 * This method reads Scene information from a block of tokens.
	 * The format for Scene blocks is described above, and any errors
	 * in parsing the block will terminate the program.
	 *
	 * \param tokens The queue of unparsed Tokens to be interpreted.
	 */
	void parseSceneBlock(std::queue<Token>& tokens);

	/** \brief Parse a block of tokens representing a Camera.
	 *
	 * This method reads Camera information from a block of tokens.
	 * The format for Camera blocks is described above, and any errors
	 * in parsing the block will terminate the program.
	 *
	 * \param tokens The queue of unparsed Tokens to be interpreted.
	 */
	void parseCameraBlock(std::queue<Token>& tokens);

	/** \brief Parse a block of tokens representing a LightSource.
	 *
	 * This method reads LightSource information from a block of tokens.
	 * The format for LightSource blocks is described above, and any errors
	 * in parsing the block will terminate the program.
	 *
	 * \param tokens The queue of unparsed Tokens to be interpreted.
	 * \return A pointer to the LightSource object read from the tokens.
	 */
	std::shared_ptr<LightSource> parseLightBlock(std::queue<Token>& tokens);

	/** 
	 * \brief Parse a block of tokens representing an Object.
	 * 
	 * This method reads Object information from a block of tokens.
	 * The format for Object blocks is described above, and any errors
	 * in parsing the block will terminate the program.
	 *
	 * \param tokens The queue of unparsed Tokens to be interpreted.
	 * \return A pointer to the Object object read from the tokens. 
	 */
	std::shared_ptr<Object> parseObjectBlock(std::queue<Token>& tokens);

	/** \brief Parse a block of tokens representing a Material.
	 *
	 * This method reads Material information from a block of tokens.
	 * The format for Material blocks is described above, and any errors
	 * in parsing the block will terminate the program.
	 *
	 * \param tokens The queue of unparsed Tokens to be interpreted.
	 */
	void parseMaterialBlock(std::queue<Token>& tokens);

	/**
	 * \brief Parse a block of tokens representing an ObjectGroup.
	 *
	 * This method reads ObjectGroup information from a block of tokens.
	 * The format for ObjectGroup blocks is described above, and any errors
	 * in parsing the block will terminate the program.
	 *
	 * \param tokens The queue of unparsed Tokens to be interpreted.
	 * \return A pointer to the ObjectGroup Object read from the tokens.
	 */
	std::shared_ptr<Object> parseObjectGroupBlock(std::queue<Token>& tokens);

	/**
	 * \brief Parse a block of tokens representing a CSG Operation.
	 *
	 * This method reads CSG information from a block of tokens.
	 * The format for CSG blocks is described above, and any errors
	 * in parsing the block will terminate the program.
	 *
	 * \param tokens The queue of unparsed Tokens to be interpreted.
	 * \return A pointer to the CSG Object read from the tokens.
	 */
	std::shared_ptr<Object> parseCSGBlock(std::queue<Token>& tokens);

	Scene* scene; //!< The Scene which information is read to.
	std::vector <std::string>tokens; //!< The queue of unparsed Tokens
	std::map<std::string, Material> materials; //!< A dictionary of Material types that have been read, and which can be used for subsequent Object properties.
	
};



#endif