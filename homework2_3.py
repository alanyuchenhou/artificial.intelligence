"""
1.3
@author: Yuchen Hou
"""
import pydot # import pydot or you're not going to get anywhere my friend :D
g1 = pydot.Dot(graph_type='graph')
g1.add_node(pydot.Node('0', label='max;10'))
g1.add_node(pydot.Node('01', label='min;10'))
g1.add_node(pydot.Node('02', label='min;4'))
g1.add_node(pydot.Node('03', label='min;8'))
g1.add_edge(pydot.Edge('0', '01', label="a1"))
g1.add_edge(pydot.Edge('0', '02', label="a2"))
g1.add_edge(pydot.Edge('0', '03', label="a3"))

g1.add_node(pydot.Node('011', label='max;10'))
g1.add_node(pydot.Node('012', label='max;11'))
g1.add_edge(pydot.Edge('01', '011'))
g1.add_edge(pydot.Edge('01', '012'))

g1.add_node(pydot.Node('021', label='max;4'))
g1.add_node(pydot.Node('022', label='x'))
g1.add_node(pydot.Node('023', label='x'))
g1.add_edge(pydot.Edge('02', '021'))
g1.add_edge(pydot.Edge('02', '022'))
g1.add_edge(pydot.Edge('02', '023'))

g1.add_node(pydot.Node('031', label='max;8'))
g1.add_node(pydot.Node('032', label='x'))
g1.add_edge(pydot.Edge('03', '031'))
g1.add_edge(pydot.Edge('03', '032'))

g1.add_node(pydot.Node('0111', label='min;6'))
g1.add_node(pydot.Node('0112', label='min;10'))
g1.add_edge(pydot.Edge('011', '0111'))
g1.add_edge(pydot.Edge('011', '0112'))

g1.add_node(pydot.Node('0121', label='min;11'))
g1.add_node(pydot.Node('0122', label='x'))
g1.add_edge(pydot.Edge('012', '0121'))
g1.add_edge(pydot.Edge('012', '0122'))

g1.add_node(pydot.Node('0211', label='min;4'))
g1.add_node(pydot.Node('0212', label='min;0'))
g1.add_edge(pydot.Edge('021', '0211'))
g1.add_edge(pydot.Edge('021', '0212'))

g1.add_node(pydot.Node('0221', label='x'))
g1.add_node(pydot.Node('0222', label='x'))
g1.add_edge(pydot.Edge('022', '0221'))
g1.add_edge(pydot.Edge('022', '0222'))

g1.add_node(pydot.Node('0231', label='x'))
g1.add_node(pydot.Node('0232', label='x'))
g1.add_edge(pydot.Edge('023', '0231'))
g1.add_edge(pydot.Edge('023', '0232'))

g1.add_node(pydot.Node('0311', label='min;5'))
g1.add_node(pydot.Node('0312', label='min;8'))
g1.add_node(pydot.Node('0313', label='min;7'))
g1.add_edge(pydot.Edge('031', '0311'))
g1.add_edge(pydot.Edge('031', '0312'))
g1.add_edge(pydot.Edge('031', '0313'))

g1.add_node(pydot.Node('0321', label='x'))
g1.add_node(pydot.Node('0322', label='x'))
g1.add_edge(pydot.Edge('032', '0321'))
g1.add_edge(pydot.Edge('032', '0322'))

g1.write_png('homework2_3.png')
