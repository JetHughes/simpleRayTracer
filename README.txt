On linux with gcc 12 `abs` returns integers. I switched most uses of `abs` to `fabs`

ParseSceneBlock is modified to take an additional parameter supersampling. If the 
keyword supersampling is present in the sceneblock, supersampling is used. Otherwise 
it is not

![image](renders/simplenosuper.png?raw=true)

