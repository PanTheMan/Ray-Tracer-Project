mat1 = gr.material({1, 0.25, 0.0}, {0.0, 0.0, 0.0}, 0.0,0.0, 0.0,0.0)
mat2 = gr.material({0.8,0.0,0.0},{1,1,1},10,2,100,0)
mat3 = gr.material({0.8,0.8,0.8},{0.0,0.0,0.0},0,0,0,0)

scene_root = gr.node('root')
scene_root:rotate('x',10)

steldodec = gr.mesh( 'dodec', 'smstdodeca.obj' )
steldodec:set_material(mat3)
scene_root:add_child(steldodec)


white_light = gr.light({-1.0, 1.5, 4.0}, {0.9, 0.9, 0.9}, {1, 0, 0})
orange_light = gr.light({4.0, 1.0, 1.5}, {0.0, 0.0, 0.0}, {1, 0, 0}, {404.0,100.0,150.0},{404.0,104.0,150.0},{396.0,104.0,150.0},{396.0,96.0,150.0})

gr.render(scene_root, 'pic1.png', 512, 512,
      {0, 0, 800}, {0, 0, -1}, {0, 1, 0}, 50,
      {0.3, 0.3, 0.3}, {white_light, orange_light})

