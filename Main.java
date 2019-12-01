import java.util.Scanner;
import java.util.ArrayList;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.InputMismatchException;

public class Main {
    public static void main(String[] args) {
        Scanner in = new Scanner(System.in); 

        try {
            int n = in.nextInt(); 
            int m = in.nextInt(); 
            int numberOfUsers = in.nextInt(); 
            
            in.nextLine();

            Grid seatGrid = new Grid(n, m);
            //seatGrid.printGrid();
    
            ArrayList<User> userList = createUserList(in, numberOfUsers, seatGrid);
            //printUserList(userList);
    
            //ExecutorService executor = Executors.newFixedThreadPool(numberOfUsers);
            
            Logger.InitLogger();

            for (User user: userList) {

                //executor.execute(user);
                //System.out.println("Executing user " + user.getUserName());
                try {
                    Thread userThread = new Thread(user);
                    userThread.start();
                    userThread.join();
                } catch (InterruptedException e) {
                    System.out.print(e.getMessage());
                }
            }

            //executor.shutdown();

            seatGrid.printGridState();
    
        } catch (InputMismatchException e) {
            System.out.print(e.getMessage());
        }

    }

    public static ArrayList<User> createUserList(Scanner in, int numberOfUsers, Grid seatGrid) {
        ArrayList<User> userList = new ArrayList<User>();

        for (int i = 0; i < numberOfUsers; i++) {
            String li = in.nextLine();

            String[] tokens = li.split("[ ]");

            User user = new User();

            String userName = tokens[0];
            user.setUserName(userName);

            int numberOfSeats = tokens.length - 1;

            ArrayList<String> seats = new ArrayList<String>();

            for (int j = 1; j < numberOfSeats + 1; j++) {
                seats.add(tokens[j]);
            }

            user.setWantedSeats(seats);
            user.setGrid(seatGrid);
            userList.add(user);
        }
        return userList;
    }

    public static void printUserList(ArrayList<User> userList) {
        for(User user: userList) {
            user.printUser();
        }
    }

}