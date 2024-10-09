from dataclasses import dataclass, field
from collections import deque


@dataclass(repr=False)
class TreeNode:
    value: str
    children: list = field(default_factory=list)

    def __repr__(self):
        return self.value

    def __hash__(self):
        return hash(self.value)

    def __lt__(self, other):
        if not isinstance(other, TreeNode):
            return NotImplemented
        return self.value < other.value


def postorder_3color_recursive(root):
    """Return a post-order ordering of nodes in the graph alongside the "back edges"."""

    NEW = 0
    SEEN = 1
    DONE = 2  # have beed fully handled
    color = dict()
    order = []
    back_edges = []

    def dfs_walk(node):
        color[node] = SEEN

        for child in reversed(node.children):
            col = color.get(child, NEW)
            if col == SEEN:
                back_edges.append((node, child))
            elif col == NEW:
                dfs_walk(child)

        order.append(node)
        color[node] = DONE

    dfs_walk(root)

    return order, back_edges


@dataclass(repr=False)
class PostOrder:
    node: TreeNode

    def __repr__(self):
        return f"PO({self.node})"


def postorder_3color_iterative(root):
    """Return a post-order ordering of nodes in the graph and "back edges" using iteration instead of recursion.
    Here the fourth color was added (compared to the recursive implementation),
    because we must know what was already added on stack and waits to be processed, to avoid duplication.
    Recursive approach does not need it because it can wait to execute the walk, since it stores information in the stack frames formed by function calls."""

    NEW = 0
    WAIT = 1  # on stack
    SEEN = 2
    DONE = 3  # have beed fully handled
    color = dict()
    order = []
    stack = deque([root])
    color[root] = WAIT

    back_edges = []

    while stack:
        node = stack.pop()

        if isinstance(node, PostOrder):
            order.append(node.node)
            color[node.node] = DONE
            continue

        color[node.value] = SEEN
        stack.append(PostOrder(node))  # will be popped after all children are visited

        for child in node.children:
            col = color.get(child, NEW)
            if col == WAIT or col == SEEN:
                back_edges.append((node, child))
            elif col == NEW:
                color[child] = WAIT
                stack.append(child)

    return order, back_edges


root = TreeNode("start")
a = TreeNode("a")
b = TreeNode("b")
c = TreeNode("c")
d = TreeNode("d")
stop = TreeNode("stop")

root.children = [a]
a.children = [b, c]
c.children = [stop]
b.children = [d]
d.children = [b, stop]

print(postorder_3color_recursive(root))
print(postorder_3color_iterative(root))

x = TreeNode("x")
t = TreeNode("t")
b = TreeNode("b")
c = TreeNode("c")
d = TreeNode("d")
e = TreeNode("e")
m = TreeNode("m")
g = TreeNode("g")

x.children = [t, b, c]
t.children = [b]
b.children = [d]
c.children = [e]
e.children = [m, d]
m.children = [c]
d.children = [g]
g.children = [d]

print(postorder_3color_recursive(x))
print(postorder_3color_iterative(x))

c3_1 = TreeNode("1")
c3_2 = TreeNode("2")
c3_3 = TreeNode("3")

c3_1.children = [c3_2, c3_3]
c3_2.children = [c3_1, c3_3]
c3_3.children = [c3_1, c3_2]

print(postorder_3color_recursive(c3_1))
print(postorder_3color_iterative(c3_1))

c5_1 = TreeNode("1")
c5_2 = TreeNode("2")
c5_3 = TreeNode("3")
c5_4 = TreeNode("4")
c5_5 = TreeNode("5")

c5_1.children = [c5_2, c5_3, c5_4, c5_5]
c5_2.children = [c5_1, c5_3, c5_4, c5_5]
c5_3.children = [c5_1, c5_2, c5_4, c5_5]
c5_4.children = [c5_1, c5_2, c5_3, c5_5]
c5_5.children = [c5_1, c5_2, c5_3, c5_4]

print(postorder_3color_recursive(c5_1))
print(postorder_3color_iterative(c5_1))

c5_1.children = c5_2.children = c5_3.children = c5_4.children = c5_5.children = [c5_1, c5_2, c5_3, c5_4, c5_5]

print(postorder_3color_recursive(c5_1))
print(postorder_3color_iterative(c5_1))


n1 = TreeNode("1")
n2 = TreeNode("2")
n3 = TreeNode("3")
n4 = TreeNode("4")
n5 = TreeNode("5")
n6 = TreeNode("6")
n7 = TreeNode("7")
n8 = TreeNode("8")

n1.children = [n2]
n2.children = [n3, n7]
n3.children = [n5, n4]
n4.children = [n6]
n5.children = [n6]
n6.children = [n3]
n7.children = [n8]

print(postorder_3color_recursive(n1))
print(postorder_3color_iterative(n1))





