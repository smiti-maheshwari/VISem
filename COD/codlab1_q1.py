#!user/bin/pythoni

class DFA:

	def __init__(self,keywords):
		self.keywords = keywords

	def CreateDFA(self):
		root=dict()
		self.end = '_end_'
		for word in self.keywords:
			current_dict = root
			for letter in word:
				current_dict = current_dict.setdefault(letter,{})
			current_dict[self.end] = self.end
		self.dfa = root
		print self.dfa

	def checkword(self,word):
		current_dict = self.dfa
		for letter in word:
			if letter in current_dict:
				current_dict = current_dict[letter]
			else:
				return False
		else:
			if self.end in current_dict:
				return True
			else:
				return False

keywords = ['for','if','while','do','main','int','char','double','float','struct']
dfa = DFA(keywords)
dfa.CreateDFA()
x=raw_input("Enter the keyword:")
print dfa.checkword(x)
