import plot.density_evo as de
import plot.order_parameter_evo as ope



def DataAnalysis():

    print("What do you want to do?")
    print("1) Plot the density evolution of a saved simulation")
    print("2) Plot the order parameter dependency on K for a specified condition")
    print("3) Return to the main menu")
    
    choice = input("Please type your choice: ")
    while(choice != "1" and choice != "2" and choice != "3"):

        print("Command not valid, please try again: ")
        print("1) Plot the density evolution of a saved simulation")
        print("2) Plot the order parameter dependency on K for a specified condition")
        print("3) Return to the main menu")
        choice = input("Please type your choice: ")
    
    if choice == "1":
        de.densityEvolution()
    
    elif choice == "2":
        ope.orderParameterEvolution()
    
    elif choice == "3":
        return