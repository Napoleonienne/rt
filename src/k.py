from random import randint

class jeu:
    def __init__(self,n=15,inteogateur=True):
        self.plage = n
        self.inteogateur =inteogateur

    def changer_role(self):
        self.inteogateur = not self.inteogateur

    def set_plage(self,n:int):
        self.plage =n
    
    def jouer(self):
        self.plage( int(input("choisisez la borne superieur")) )

        if self.inteogateur:
            self.abitre()
        else:
            self.participant()

    def participant(self):
        mini = 0
        maxi = self.plage
        print("role: participant  ")

        nombre = int(input(f"veuillez choisir un nombre entre 0 et {self.plage} : "))
        
        c =  self.plage
        question =0
        while c !=nombre:

            c = round(c/2)
            print(c)
            d = str(input("choisisez entre + et - : "))
            match d:
                case "-":
                    c = mini-c
                    maxi = c
                case "+":
                    c = c + maxi
                    mini = c
                    

                case _ :
                    print(" input inconu recomecez")
            question  +=1
        print(question)
                    


    def abitre(self):
        c = -1
        nombre = randint(0,self.plage)
        question = 0

        while c !=nombre:
            c = int(input(f"veuillez choisir un nombe entre 0 et {self.plage} : "))
            if type(c) ==int:
                if c < nombre:
                    print("+")
                elif c > nombre:
                    print("-")
            question +=1

        print(question)




            
        
jeu_instance = jeu(n=1023)
jeu_instance.changer_role()
jeu_instance.jouer()