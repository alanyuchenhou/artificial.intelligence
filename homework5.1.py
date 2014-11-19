"""
@author: Yuchen Hou
"""
import pydot
from pydot import Dot
from pydot import Node
from pydot import Edge
g1 = pydot.Dot(graph_type='graph')
g1.add_node(pydot.Node('0', label='Cargo'))
g1.add_node(pydot.Node('01', label='west'))
g1.add_node(pydot.Node('02', label='west'))
g1.add_node(pydot.Node('03', label='Circels'))
g1.add_node(pydot.Node('04', label='east'))
g1.add_edge(pydot.Edge('0', '01', label="1"))
g1.add_edge(pydot.Edge('0', '02', label="2"))
g1.add_edge(pydot.Edge('0', '03', label="3"))
g1.add_edge(pydot.Edge('0', '04', label="4"))

g1.add_node(pydot.Node('030', label='east'))
g1.add_node(pydot.Node('031', label='east'))
g1.add_node(pydot.Node('032', label='Cars'))
g1.add_node(pydot.Node('033', label='west'))
g1.add_edge(pydot.Edge('03', '030', label='0'))
g1.add_edge(pydot.Edge('03', '031', label='1'))
g1.add_edge(pydot.Edge('03', '032', label='2'))
g1.add_edge(pydot.Edge('03', '033', label='3'))

g1.add_node(pydot.Node('0321', label='east'))
g1.add_node(pydot.Node('0322', label='west'))
g1.add_edge(pydot.Edge('032', '0321', label='3'))
g1.add_edge(pydot.Edge('032', '0322', label='4'))

g1.write_png('homework5_1.png')
