--Colours
gold = gr.material({0.723,0.578,0.269},{0.2,0.2,0.2},20,0,0,0)
red = gr.material({0.8,0,0},{0.8,0.1,0.1}, 20, 0, 0.0,0.5)
blue = gr.material({0,0,0.8},{0.1,0.1,0.8}, 20, 0, 0.0,0.5)
green = gr.material({0,0.8,0},{0.1,0.8,0.1}, 20, 0, 0.0,0.5)
black = gr.material({0,0,0},{0.1,0.1,0.1}, 20, 0, 0.0,0)
yellow = gr.material({0.9,0.8,0.4},{0.9,0.8,0.4}, 20, 1.5, 0.0,0.5)
empty = gr.material({0,0,0},{0,0,0},0,0,0,0)
redWall = gr.material( {0.44, 0.137, 0.1886}, {0.05, 0.05, 0.05}, 25, 0.0,0.0,0.0)
greenWall = gr.material( {0.1386, 0.1327, 0.45}, {0.05, 0.05, 0.05}, 25, 0.0,0.0,0.0)
greyWall = gr.material( {0.4649, 0.4823, 0.5002}, {0.3, 0.3, 0.3}, 25, 0.0,0.0,0.0)

scene = gr.node('root')

s3 = gr.sphere('s3')
scene:add_child(s3)
s3:scale(3,3,3)
s3:translate(0,6.7,-15)
s3:set_material(black)
s3:set_texture("./Assets/BasketballColor.png")

s4 = gr.sphere('s4')
scene:add_child(s4)
s4:scale(2,2,2)
s4:translate(0,1,-25)
s4:set_material(black)
s4:set_texture("./Assets/baseball.png")

s5 = gr.sphere('s5')
scene:add_child(s5)
s5:translate(0,-2,-5.5)
s5:set_material(black)
s5:set_texture("./Assets/tennis.png")

floor = gr.mesh('floor','./Assets/plane.obj')
scene:add_child(floor)
floor:set_material(empty)
floor:scale(20, 20, 20)
floor:translate(0, -10, -20)
floor:set_texture("./Assets/hardwoodfloor.png")

chessboard = gr.cube("chess")
floor:add_child(chessboard)
chessboard:set_material(empty)
chessboard:rotate('x',270)
chessboard:rotate('y',30)
chessboard:translate(-0.2,-0.75,1)
chessboard:scale(0.5,0.25,1/3.0)
chessboard:set_texture("./Assets/chessboard.png")

backwall = gr.mesh( 'backwall', './Assets/plane.obj' )
scene:add_child(backwall)
backwall:set_material(greyWall)
backwall:rotate('X', 90)
backwall:scale(10, 10, 100)
backwall:translate(0, 0, -26)
backwall:set_texture("./Assets/blueWallpaper.png")

leftwall = gr.mesh('leftwall', './Assets/plane.obj')
scene:add_child(leftwall)
leftwall:set_material(redWall)
leftwall:rotate('Z', -90)
leftwall:scale(10, 10, 30)
leftwall:translate(-10, 0, -10)
leftwall:set_texture("./Assets/blueWallpaper.png")

rightwall = gr.mesh('rightwall','./Assets/plane.obj')
scene:add_child(rightwall)
rightwall:set_material(greenWall)
rightwall:rotate('Z', 90)
rightwall:scale(10, 10, 30)
rightwall:translate(10, 0, -10)
rightwall:set_texture("./Assets/blueWallpaper.png")

topwall = gr.mesh('topwall','./Assets/plane.obj')
scene:add_child(topwall)
topwall:set_material(greyWall)
topwall:rotate('X', 180)
topwall:scale(10, 10, 30)
topwall:translate(0, 10, -10)
topwall:set_texture("./Assets/milkyway.png")



white_light = gr.light({-1.0, 1.5, 10.0}, {0.9,0.9,0.9}, {1, 0, 0})
orange_light = gr.light({4.0, 1.0, 1.5}, {0.0, 0.0, 0.0}, {1, 0, 0}, {404.0,100.0,150.0},{404.0,104.0,150.0},{396.0,104.0,150.0},{396.0,96.0,150.0})

gr.render(scene, 'DOF.png', 512, 512,
      {0, 0, 5}, {0, 0, -1}, {0, 1, 0}, 50,
      {0.3, 0.3, 0.3}, {white_light,orange_light})

 
