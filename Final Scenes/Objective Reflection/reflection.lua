glass = gr.material( {0.05, 0.05, 0.05}, {0.7, 0.7, 0.7}, 25, 1.5, 10000.0,0.0)
redWall = gr.material( {0.44, 0.137, 0.1886}, {0.05, 0.05, 0.05}, 25, 0.0,0.0,0.0)
greenWall = gr.material( {0.1386, 0.1327, 0.45}, {0.05, 0.05, 0.05}, 25, 0.0,0.0,0.0)
--grey_wall = gr.material( {0.4649, 0.4823, 0.5002}, {0.3, 0.3, 0.3}, 25, 0.0,0.0)
greyWall = gr.material( {0.4649, 0.4823, 0.5002}, {0.3, 0.3, 0.3}, 25, 0.0,0.0,0.0)
empty = gr.material({0,0,0},{0,0,0},0,0,0,0)

mirror = gr.material( {0.0, 0.0, 0.0},   {1.0, 1.0, 1.0},	25, 1.5,0.0,0.8)
red = gr.material({0.8,0,0},{0.8,0.1,0.1}, 20, 1.5, 0.0,0.5)
blue = gr.material({0,0,0.8},{0.1,0.1,0.8}, 20, 1.5, 0.0,0.5)
green = gr.material({0,0.8,0},{0.1,0.8,0.1}, 20, 1.5, 0.0,0.5)
black = gr.material({0,0,0},{0.1,0.1,0.1}, 20, 1.5, 0.0,0.5)
yellow = gr.material({0.9,0.8,0.4},{0.9,0.8,0.4}, 20, 1.5, 0.0,0.5)


scene = gr.node('scene')

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
chessboard:rotate('y',30)
chessboard:translate(-0.2,-0.65,1)
chessboard:scale(1,0.5,1/3.0)
chessboard:set_texture("./Assets/chessboard.png")

center = gr.sphere('center')
scene:add_child(center)
center:set_material(mirror)
center:scale(2, 2, 2)

left = gr.sphere('left')
center:add_child(left)
left:set_material(red)
left:translate(-2.5,0,0)

right = gr.sphere('right')
center:add_child(right)
right:set_material(blue)
right:translate(2.5,0,0)

up = gr.sphere('up')
center:add_child(up)
up:set_material(green)
up:translate(0,2.5,0)

down = gr.sphere('down')
center:add_child(down)
down:set_material(yellow)
down:translate(0,-2.5,0)

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
leftwall:set_texture("./Assets/blueWallpaper.png")

rightwall = gr.mesh('rightwall','plane.obj')
scene:add_child(rightwall)
rightwall:set_material(greenWall)
rightwall:rotate('Z', 90)
rightwall:scale(10, 10, 30)
rightwall:translate(10, 0, 0)
rightwall:set_texture("./Assets/blueWallpaper.png")

topwall = gr.mesh('topwall','plane.obj')
scene:add_child(topwall)
topwall:set_material(greyWall)
topwall:rotate('X', 180)
topwall:scale(10, 10, 30)
topwall:translate(0, 10, 0)
topwall:set_texture("./Assets/milkyway.png")

l1 = gr.light({2, 2, 6}, {0.8, 0.8, 0.8}, {1, 0, 0}) --{2.5,2,5}, {2.5,2,5.5}, {1.5,2,5.5},{1.5,2,4.5})
white_light = gr.light({-1.0, 1.5, 8.0}, {0.9,0.9,0.9}, {1, 0, 0})

gr.render(scene, 'reflection.png', 512, 512,
	  {0, 0, 30}, {0, 0, -1}, {0, 1, 0}, 50,
	  {0.4, 0.4, 0.4},{l1})

