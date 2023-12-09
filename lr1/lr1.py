from grammar import *
import queue

class LR1:
    class Situation:
        def __init__(self, rule: Rule, char: str, point_position: int):
            self.rule = rule
            self.char = char
            self.point_position = point_position
            
        def __eq__(self, other) -> bool:
            return self.rule == other.rule and self.i == other.i and self.point_position == other.point_position
    
        def __ne__(self, other) -> bool:
            return not self.__eq__(other)
        
        def __hash__(self):
            return hash((self.rule, self.char, self.point_position))
        
        def passed(self):
            return len(self.rule.right) == self.point_position
        
    class Node:
        def __init__(self):
            self.situations = set()
            self.index = 0
            self.reducable = False
            
        def __eq__(self, other) -> bool:
            return self.situations == other.situations
            
        def __ne__(self, other) -> bool:
            return not self.__eq__(other)
            
        def __hash__(self):
            return hash((self.situations))
        
        def add_situation(self, situation):
            situation.add(situation)
        
    class Action:
        def __init__(self, type: str, value: int) -> None:
            self.type = type
            self.value = value
        
    def __init__(self, grammar: Grammar):
        self.grammar = grammar
        self.nodes = set()
        self.graph = list()
        self.nodes_helper = list()

    
    def closure(self, node) -> None:
        prev_len = len(node)
        while True:
            for situation in node.situations:
                if situation.passed():
                    node.reducable = True
                if not(situation.passed()) and self.grammar.isNonterm(situation.rule.right[situation.point_position]):
                    for new_rule in self.grammar.rules[situation.rule.right[situation.point_position]]:
                        for new_char in self.grammar.First(situation.rule.right[situation.point_position:] + situation.char):
                            node.situations.add(self.Situation(new_rule, new_char, 0))
            
            cur_len = len(node)
            if cur_len == prev_len:
                break
            prev_len = cur_len
    
    def goto(self, node, char: str) -> None:
        new_node = self.Node()
        for elem in node.situations:
            if not(elem.passed()) and elem.rule.right[elem.point_position] == char:
                new_situation = self.Situation(elem.rule, elem.char, elem.point_position + 1)
                new_node.add_situation(new_situation)
        
        self.closure(new_node)
        
        if new_node not in self.nodes:
            new_node.index = len(self.nodes)
            self.nodes.add(new_node)
            self.nodes_helper.append(new_node)
            self.graph[node.index].append((new_node.index, char))
        
    def get_symbols_for_goto(self, node) -> set:
        result = set()
        for elem in node.situations:
            if not(elem.passed):
                result.add(elem.rule.right[elem.point_position])
                
        return result
 
    def build_graph(self, word: str):
        first_node = self.Node()
        first_node.add_situation(self.Situation(Rule("?", self.grammar.start_nonterm), '$', 0))
        self.closure(first_node)
        self.nodes.add(first_node)
        self.nodes_helper.append(first_node)
        self.graph.append(list())
        node_queue = queue()
        while not(node_queue.empty()):
            prev_len = len(self.nodes)
            cur_node = node_queue.get()
            for symbol in self.get_symbols_for_goto(cur_node):
                self.goto(cur_node, symbol)
            
            for i in range(prev_len, len(self.nodes)):
                node_queue.put(self.nodes_helper[i])
        
    def build_table(self):
        self.table = [dict()] * len(self.nodes)
        for node_from in range(len(self.graph)):
            for elem in self.graph[node_from]:
                to, char = elem
                if self.grammar.isNonterm(char):
                    self.table[node_from][char] = self.Action("default", to)
                elif self.grammar.isTerm(char):
                    self.table[node_from][char] = self.Action("shift", to)
                else:
                    raise ValueError(f"build table: symbol {char} not in Non-terms and not in Terms")
        
        for i in len(self.nodes_helper):
            node = self.nodes_helper[i]
            if node.reducable:
                for situation in node.situations:
                    if situation.passed():
                        index = self.grammar.rule_helper.index((situation.rule.left, situation.rule.right))
                        if char not in self.table[i].keys():
                            self.table[i][situation.char] = self.Action("reduce", index)  
        
    def process(self, word: str) -> bool:
        word = word + '$'
        pos = 0
        stack_char = []
        stack_int = [0]
        while pos < len(word):
            a = word[pos]
            q = stack_int[-1]
            if a not in self.table[q].keys():
                return False
            match self.table[q][a].type:
                case "shift":
                    stack_char.append(a)
                    pos += 1
                    stack_int.append(self.table[q][a].value)
                case "reduce":
                    if self.table[q][a].value == 0:
                        return True
                    
                    left, right = self.grammar.rule_helper[self.table[q][a].value]
                    if len(stack_char) < len(right):
                        return False
                    
                    test = ""
                    for i in range(len(stack_char) - len(right)):
                        test = test + stack_char[i]
                        
                    if test != right:
                        return False
                
                    for i in range(len(stack_char)):
                        stack_char.pop()
                        stack_int.pop()
                    
                    stack_char.append(left)
                    if stack_int.emtpy():
                        return False
                    if left not in self.table[stack_int[-1]].keys():
                        return False

                    stack_int.append(self.table[stack_int[-1]][left].value)
                    
                case _:
                    return False
        
    def inGrammar(self, word: str) -> bool:
        print(f"\n\nNEW QUERY \"{word}\"\n\n")
        return self.process(word)
