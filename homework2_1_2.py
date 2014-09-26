"""
1.3
@author: Yuchen Hou
"""
import pydot # import pydot or you're not going to get anywhere my friend :D
# graph = pydot.Dot(graph_type='graph')
# for i in range(4):
#     edge = pydot.Edge("king", "lord%d" % i)
#     graph.add_edge(edge)
#     vassal_num = 0
#     for j in range(2):
#         edge = pydot.Edge("lord%d" % i, "vassal%d" % vassal_num)
#         graph.add_edge(edge)
#         vassal_num += 1
# graph.write_png('1.3.png')
graph_a = pydot.Dot(graph_type='graph')
n0 = "1,1,r; 0+2^2+1=5;"
n01 = "1,2,r; 1+1^2+1=3;"
n02 = "1,1,u; 1+2^2+0=5;"
n03 = "1,1,d; 1+2^2+2=7;"
graph_a.add_edge(pydot.Edge(n0,n01))
graph_a.add_edge(pydot.Edge(n0,n02))
graph_a.add_edge(pydot.Edge(n0,n03))
n011 = "1,3,r; 2+2^2+1=7"
n012 = "1,2,u; 2+1^2+0=3"
n013 = "1,2,d; 2+1^2+2=5"
graph_a.add_edge(pydot.Edge(n01,n011))
graph_a.add_edge(pydot.Edge(n01,n012))
graph_a.add_edge(pydot.Edge(n01,n013))
n021 = "2,1,u"
n022 = "1,1,l"
n023 = "1,1,r"
# graph_a.add_edge(pydot.Edge(n02,n021))
# graph_a.add_edge(pydot.Edge(n02,n022))
# graph_a.add_edge(pydot.Edge(n02,n023))
n031 = "1,1,d"
n032 = "1,1,r"
n033 = "1,1,l"
# graph_a.add_edge(pydot.Edge(n03,n031))
# graph_a.add_edge(pydot.Edge(n03,n032))
# graph_a.add_edge(pydot.Edge(n03,n033))
n0111 = "1,4,r"
n0112 = "1,3,u"
n0113 = "1,3,d"
# graph_a.add_edge(pydot.Edge(n011,n0111))
# graph_a.add_edge(pydot.Edge(n011,n0112))
# graph_a.add_edge(pydot.Edge(n011,n0113))
n0121 = "2,2,u; 3+0^2+0=3"
n0122 = "1,2,l; 3+1^2+1=5"
n0123 = "1,2,r; 3+1^2+1=5"
graph_a.add_edge(pydot.Edge(n012,n0121))
graph_a.add_edge(pydot.Edge(n012,n0122))
graph_a.add_edge(pydot.Edge(n012,n0123))

graph_a.write_pdf('homework2_1_2.png')
