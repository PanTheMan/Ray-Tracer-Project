scene = gr.node('scene')
white_light = gr.light({0, 0, 30}, {0.9,0.9,0.9}, {1, 0, 0})
empty = gr.material({0,0,0},{0,0,0},0,0,0,0)
grey = gr.material({0.7,0.7,0.7},{0,0,0},0,0,0,0)
bunnyfur = gr.material({1,0.6,0.2},{0,0,0},0,0,0,0)

rabbit = gr.mesh("rabbit", "bunny.obj")
scene:add_child(rabbit)
rabbit:scale(30,30,30)
rabbit:translate(0.5,-3.8,4)
rabbit:set_material(bunnyfur)

floor = gr.mesh('floor','plane.obj')
scene:add_child(floor)
floor:set_material(grey)
floor:scale(20, 20, 30)
floor:translate(0, -10, 2)
floor:set_texture("./Assets/hardwoodfloor.png")
gr.render(scene, 'AntiAlias.png', 512, 512,
	  {0, 0, 20}, {0, 0, -1}, {0, 1, 0}, 50,
	  {0.4, 0.4, 0.4},{white_light})

