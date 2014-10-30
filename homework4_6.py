"""
@author: Yuchen Hou
"""
import pydot
g1 = pydot.Dot(graph_type='digraph')
g1.add_node(pydot.Node('p13', label='p13'))
g1.add_node(pydot.Node('p22', label='p22'))
g1.add_node(pydot.Node('p31', label='p31'))
g1.add_node(pydot.Node('b12', label='b12'))
g1.add_node(pydot.Node('b21', label='b21'))
g1.add_edge(pydot.Edge('p13', 'b12'))
g1.add_edge(pydot.Edge('p22', 'b12'))
g1.add_edge(pydot.Edge('p22', 'b21'))
g1.add_edge(pydot.Edge('p31', 'b21'))
g1.write_png('homework4_6.png')
