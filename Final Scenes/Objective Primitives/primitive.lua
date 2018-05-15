glass = gr.material( {0.05, 0.05, 0.05}, {0.7, 0.7, 0.7}, 25, 1.5, 10000.0,0.0)	
redWall = gr.material( {0.44, 0.137, 0.1886}, {0.05, 0.05, 0.05}, 25, 0.0,0.0,0.0)
greenWall = gr.material( {0.1386, 0.1327, 0.45}, {0.05, 0.05, 0.05}, 25, 0.0,0.0,0.0)
greyWall = gr.material( {0.4649, 0.4823, 0.5002}, {0.3, 0.3, 0.3}, 25, 0.0,0.0,0.0)
empty = gr.material({0,0,0},{0,0,0},0,0,0,0)
mat4  = gr.material({0.7, 0.6, 1.0}, {0.5, 0.4, 0.8}, 25, 0.0,0.0,0.0)
mat1  = gr.material({0.7, 1.0, 0.7}, {0.5, 0.7, 0.5}, 25, 0.0,0.0,0.0)
mat3  = gr.material({1.0, 0.6, 0.1}, {0.5, 0.7, 0.5}, 25, 0.0,0.0,0.0)
mirror = gr.material( {0.0, 0.0, 0.0},   {1.0, 1.0, 1.0},	25, 1.3,0.0,0.8)

red = gr.material({0.8,0,0},{0.1,0.1,0.1}, 20, 0, 0.0,0.0)
blue = gr.material({0,0,0.8},{0.1,0.1,0.1}, 20, 0, 0.0,0.0)
green = gr.material({0,0.8,0},{0.1,0.1,0.1}, 20, 0, 0.0,0.0)
yellow = gr.material({0.9,0.8,0.4},{0.1,0.1,0.1}, 20, 0, 0.0,0.0)


scene = gr.node('scene')
scene:rotate('x',10)

floor = gr.mesh('floor','plane.obj')
scene:add_child(floor)
floor:set_material(greyWall)
floor:scale(10, 10, 30)
floor:translate(0, -10, 0)
floor:set_texture("./Assets/hardwoodfloor.png")

chessboard = gr.cube("chess")
floor:add_child(chessboard)
chessboard:set_material(empty)
chessboard:rotate('x',270)
chessboard:rotate('y',40)
chessboard:translate(0,-0.65,0.85)
chessboard:scale(1,0.5,1/3.0)
chessboard:set_texture("./Assets/chessboard.png")

cube = gr.cube("cube")
chessboard:add_child(cube)
-- cube:scale(1,1,1)

cube:scale(0.25,0.25,0.5)
cube:translate(0.1,0.4,1)
cube:set_material(red)

cylinder = gr.cylinder("cylinder")
chessboard:add_child(cylinder)
cylinder:rotate('x',90)
cylinder:scale(0.1,0.1,0.2)
cylinder:translate(0.26,0.2,1.2)
cylinder:set_material(blue)

cone = gr.cone("cone")
chessboard:add_child(cone)
--cone:rotate('x',-90)
cone:scale(0.12,0.3,0.24)
cone:translate(0.65,0,1.25)
cone:set_material(green)

sphere = gr.sphere("sphere")
chessboard:add_child(sphere)
sphere:rotate('x',90)
sphere:scale(0.15,0.15,0.3)
sphere:translate(0.65,0.55,1.3)
sphere:set_material(yellow)




backwall = gr.mesh( 'backwall', 'plane.obj' )
scene:add_child(backwall)
backwall:set_material(greyWall)
backwall:rotate('X', 90)
backwall:scale(10, 10, 100)
backwall:translate(0, 0, -10)
backwall:set_texture("./Assets/blueWallpaper.png")

leftwall = gr.mesh('leftwall', 'plane.obj')
scene:add_child(leftwall)
leftwall:set_material(redWall)
leftwall:rotate('Z', -90)
leftwall:scale(10, 10, 30)
leftwall:translate(-10, 0, 0)

rightwall = gr.mesh('rightwall','plane.obj')
scene:add_child(rightwall)
rightwall:set_material(greenWall)
rightwall:rotate('Z', 90)
rightwall:scale(10, 10, 30)
rightwall:translate(10, 0, 0)

topwall = gr.mesh('topwall','plane.obj')
scene:add_child(topwall)
topwall:set_material(greyWall)
topwall:rotate('X', 180)
topwall:scale(10, 10, 30)
topwall:translate(0, 10, 0)
topwall:set_texture("./Assets/milkyway.png")

l1 = gr.light({2, 2, 2}, {0.8, 0.8, 0.8}, {1, 0, 0}) --{2.5,2,5}, {2.5,2,5.5}, {1.5,2,5.5},{1.5,2,4.5})
white_light = gr.light({-1.0, 1.5, 4.0}, {0.9,0.9,0.9}, {1, 0, 0})

gr.render(scene, 'Primitive.png', 512, 512,
	  {0, 0, 30}, {0, 0, -1}, {0, 1, 0}, 50,
	  {0.4, 0.4, 0.4},{l1,white_light})

