import queue

class Rule:
    def __init__(self, left: str, right: str):
        self.left = left
        self.right = right
    
    def __eq__(self, other) -> bool:
        return (self.left == other.left) and (self.right == other.right)
    
    def __ne__(self, other) -> bool:
        return not self.__eq__(other)
    
    def __hash__(self) -> int:
        return hash((self.left, self.right))
    
class Grammar:
    def __init__(self, nonterms: set[str], terms: set[str]):
        self.nonterms = nonterms
        self.terms = terms
        self.rule_helper = list()
        terms.add('$')
        self.rules = dict()
        self.eps_generators = dict()
    
    def add_rule(self, rule: Rule):
        if rule.left not in self.rules.keys():
            self.rules[rule.left] = list()

        self.rule_helper.append((rule.left, rule.right))
        self.rules[rule.left].append(rule.right)
        
    def add_start_nonterm(self, start_nonterm: str):
        self.start_nonterm = start_nonterm

    def get_rules(self) -> set[Rule]:
        return self.rules
    
    def isNonterm(self, symbol: str):
        return symbol in self.nonterms
    
    def isTerm(self, symbol: str):
        return symbol in self.terms
    
    def isContextFree(self) -> bool:
        for start in self.rules.keys():
            if start not in self.nonterms or len(start) != 1:
                return False
        
        return True
    
    def isLR1(self) -> bool:
        
        return True
    
    def get_first(self, nonterm: str) -> set:
        if nonterm not in self.nonterms:
            raise ValueError('getting first for non-terminal or undefined symbol')

        if nonterm not in self.first.keys():
            result = set
            used = dict()
            for char in self.nonterms:
                used[char] = False
                
            char_queue = queue()
            char_queue.put(nonterm)
            while not(char_queue.empty()):
                cur_char = char_queue.get()
                used[cur_char] = True
                for right in self.grammar.rules[cur_char]:
                    if right[0] not in self.nonterms:
                        result.add(right[0])
                    elif not(used[right[0]]):
                        char_queue.put(right[0])
            self.first[nonterm] = result
            
        return self.first[nonterm]
        
    def eps_generator(self, nonterm: str) -> bool:
        if nonterm not in self.nonterms:
            raise ValueError('Checking eps-generative for undefined nonterm')
        
        if nonterm not in self.eps_generators.keys():
            self.eps_generator[nonterm] = "" in self.get_first(nonterm)
                    
        return self.eps_generators[nonterm]
    
    def First(self, alpha: str) -> set:
        if len(alpha) == 0:
            raise ValueError("Empty string given First")
        
        if alpha[0] in self.terms:
            return {alpha[0]}
        
        if alpha[0] in self.nonterms:
            if self.eps_generator(alpha[0]) and len(alpha) > 1:
                result = (self.get_first(alpha[0])).union(self.get_first(alpha[1]))
                return result
            else:
                return self.get_first(alpha[0])
        
        raise ValueError(f"Unknown string {alpha}")
        

    
    