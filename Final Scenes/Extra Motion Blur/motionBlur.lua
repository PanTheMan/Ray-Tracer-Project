glass = gr.material( {0.05, 0.05, 0.05}, {0.7, 0.7, 0.7}, 25, 1.5, 10000.0,0.0)	
redWall = gr.material( {0.44, 0.137, 0.1886}, {0.05, 0.05, 0.05}, 25, 0.0,0.0,0.0)
greenWall = gr.material( {0.1386, 0.1327, 0.45}, {0.05, 0.05, 0.05}, 25, 0.0,0.0,0.0)
--grey_wall = gr.material( {0.4649, 0.4823, 0.5002}, {0.3, 0.3, 0.3}, 25, 0.0,0.0)
greyWall = gr.material( {0.4649, 0.4823, 0.5002}, {0.3, 0.3, 0.3}, 25, 0.0,0.0,0.0)
empty = gr.material({0,0,0},{0,0,0},0,0,0,0)
mat4  = gr.material({0.7, 0.6, 1.0}, {0.3, 0.3, 0.3}, 25, 0.0,0.0,0.0)
mat1  = gr.material({0.7, 1.0, 0.7}, {0.5, 0.7, 0.5}, 25, 0.0,0.0,0.0)
mat3  = gr.material({1.0, 0.6, 0.1}, {0.5, 0.7, 0.5}, 25, 0.0,0.0,0.0)

mirror = gr.material( {0.0, 0.0, 0.0},   {1.0, 1.0, 1.0},	25, 1.3,0.0,0.8)

scene = gr.node('scene')

floor = gr.mesh('floor','./Assets/plane.obj')
scene:add_child(floor)
floor:set_material(greyWall)
floor:rotate('x',10)
floor:scale(30, 30, 30)
floor:translate(0, -10, 0)
floor:set_texture("./Assets/hardwoodfloor.png")

chessboard = gr.cube("chess")
--floor:add_child(chessboard)
chessboard:set_material(empty)
chessboard:rotate('x',270)
chessboard:rotate('y',30)
chessboard:translate(-0.2,-0.8,0)
chessboard:scale(2/3.0,1/3.0,1/3.0)
chessboard:set_texture("./Assets/chessboard.png")

s1 = gr.sphere('s1')
scene:add_child(s1)
s1:set_material(mat4)
s1:set_texture("./Assets/baseball.png")
s1:scale(1.5,1.5,1.5)
s1:translate(2,1.5,20)

s2 = gr.sphere('s2')
scene:add_child(s2)
s2:set_material(mat4)
s2:set_texture("./Assets/baseball.png")
s2:scale(1.5,1.5,1.5)
s2:translate(-2,1.5,20)


l1 = gr.light({2, 2, 2}, {0.8, 0.8, 0.8}, {1, 0, 0}) --{2.5,2,5}, {2.5,2,5.5}, {1.5,2,5.5},{1.5,2,4.5})
white_light = gr.light({0, 1.5, 25}, {0.9,0.9,0.9}, {1, 0, 0})

gr.render(scene, 'motionBlur.png', 512, 512,
	  {0, 0, 30}, {0, 0, -1}, {0, 1, 0}, 50,
	  {0.4, 0.4, 0.4},{l1,white_light})

