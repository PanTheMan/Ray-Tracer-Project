t=90
-- Colours
hide = gr.material({0.84, 0.6, 0.53}, {0.3, 0.3, 0.3}, 20,0,0,0)
glass = gr.material( {0.05, 0.05, 0.05}, {0.7, 0.7, 0.7}, 25, 1.5, 10000.0,0.0)	
--grey_wall = gr.material( {0.4649, 0.4823, 0.5002}, {0.3, 0.3, 0.3}, 25, 0.0,0.0)
greyWall = gr.material( {0.4649, 0.4823, 0.5002}, {0.3, 0.3, 0.3}, 25, 0.0,0.0,0.0)
empty = gr.material({0,0,0},{0,0,0},0,0,0,0)
mat4  = gr.material({0.7, 0.6, 1.0}, {0.5, 0.4, 0.8}, 25, 0.0,0.0,0.0)
mat1  = gr.material({0.7, 1.0, 0.7}, {0.5, 0.7, 0.5}, 25, 0.0,0.0,0.0)
mat3  = gr.material({1.0, 0.6, 0.1}, {0.5, 0.7, 0.5}, 25, 0.0,0.0,0.0)
mirror = gr.material( {0.1, 0.1, 0.1},   {0.8, 0.8, 0.8},	25, 1.3,0.0,0.6)
gold = gr.material({0.723,0.578,0.269}, {0.2,0.2,0.2},20,0,0,0)

scene = gr.node('scene')
scene:rotate('x',40)

floor = gr.mesh('floor','./Assets/plane.obj')
scene:add_child(floor)
floor:set_material(greyWall)
floor:scale(30, 10, 30)
floor:translate(0, -10, 0)
floor:set_texture("./Assets/hardwoodfloor.png")

chessboard = gr.cube("chess")
floor:add_child(chessboard)
chessboard:set_material(empty)
chessboard:rotate('x',270)
chessboard:rotate('y',30)
chessboard:translate(-0.2,-0.65,0.5)
chessboard:scale(1/1.5,1,1/3.0)
chessboard:set_texture("./Assets/chessboard.png")

backwall = gr.mesh( 'backwall', './Assets/plane.obj' )
scene:add_child(backwall)
backwall:set_material(greyWall)
backwall:rotate('X', 90)
backwall:scale(24, 20, 20)
backwall:translate(0, 2, -15)
backwall:set_texture("./Assets/night.png")

ball = gr.sphere("ball")
scene:add_child(ball)
ball:set_material(mirror)
ball:translate(-20+44*t/90,12*math.abs(math.cos(2*math.pi*t/90))*math.exp(-0.5*t/30)-9,-13)

axis = gr.cylinder("axis")
scene:add_child(axis)
axis:set_material(greyWall)
axis:rotate('y',30)
axis:scale(0.6,6,0.6)
axis:translate(0,-0.5,-3)

globe = gr.sphere("globe")
axis:add_child(globe)
globe:rotate('z',23)
globe:rotate('y',4*t)
globe:scale(1/0.15,2/3.0,1/0.15)
globe:set_material(empty)
globe:set_texture("./Assets/earth_no_clouds.png")
globe:set_bump("./Assets/download.png")

spinner = gr.mesh("spinner", "./Assets/Spinner.obj")
floor:add_child(spinner)
spinner:set_material(gold)
spinner:rotate('y',t*32)
spinner:scale(1/30,1/10,1/30) -- for undoing floor
spinner:scale(0.04,0.04,0.04)
spinner:translate(0.27,0.22,0.25)

cannon = gr.cylinder('cannon')
floor:add_child(cannon)
cannon:scale(1/30,1/10,1/30)
cannon:scale(1,3.2,1)
cannon:rotate('z',15)
cannon:rotate('y',45)
cannon:translate(0.4,0.32,-0.25)
cannon:set_material(mat1)

axel = gr.cylinder('axel')
cannon:add_child(axel)
axel:rotate('x',90)
axel:scale(0.5,0.25/1.6,1.5)
axel:set_material(greyWall)
axel:translate(-0.7,-0.8,0)

wheel1 = gr.sphere('wheel1')
axel:add_child(wheel1)
wheel1:scale(4,4,1)
wheel1:scale(0.8,0.1,3)
wheel1:set_material(mat3)
wheel1:translate(0,-1.1,0)

wheel2 = gr.sphere('wheel2')
axel:add_child(wheel2)
wheel2:scale(4,4,1)
wheel2:scale(0.8,0.1,3)
wheel2:set_material(mat3)
wheel2:translate(0,1.1,0)

cow = gr.node('the_cow')

for _, spec in pairs({
			{'body', {0, 0, 0}, 1.0},
			{'head', {.9, .3, 0}, 0.6},
			{'tail', {-.94, .34, 0}, 0.2},
			{'lfleg', {.7, -.7, -.7}, 0.3},
			{'lrleg', {-.7, -.7, -.7}, 0.3},
			{'rfleg', {.7, -.7, .7}, 0.3},
			{'rrleg', {-.7, -.7, .7}, 0.3}
		     }) do
   part = gr.nh_sphere(table.unpack(spec))
   part:set_material(hide)
   cow:add_child(part)
end
cow_instance = gr.node('cow1')
floor:add_child(cow_instance)
cow_instance:add_child(cow)
cow_instance:rotate('z',45*math.cos(t*math.pi/180))
cow_instance:rotate('y',225)
cow_instance:scale(1.4/30, 1.4/10, 1.4/30)
cow_instance:translate(0.38-0.72*t/90,0.35+1.732*math.sin(2*math.pi*t/270),-0.23+0.72*t/90)


purpleLight = gr.light({10.0, 0, 10.0}, {0.785,0.3125,0.676}, {1, 0, 0})
orangeLight = gr.light({-5.0, 10, 10.0}, {0.8085,0.4258,0.039}, {1, 0, 0})
blueLight = gr.light({0.0, 10, 4.7}, {0.3281,0.4961,0.6328}, {1, 0, 0})
gr.render(scene, 'a' .. tostring(t) .. '.png', 512,512,
	  {0, 0, 35}, {0, 0, -1}, {0, 1, 0}, 50,
	  {0.4, 0.4, 0.4},{orangeLight,purpleLight,blueLight})

