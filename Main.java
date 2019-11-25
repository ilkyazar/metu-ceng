import java.util.Scanner;
import java.util.ArrayList;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class Main {
    public static void main(String[] args) {
        Scanner in = new Scanner(System.in); 
  
        int n = in.nextInt(); 
        int m = in.nextInt(); 
        int numberOfUsers = in.nextInt(); 
  
        in.nextLine();

        Grid seatGrid = new Grid(n, m);
        //seatGrid.printGrid();
        //printUserList(userList);

        ArrayList<User> userList = createUserList(in, numberOfUsers, seatGrid);

        ExecutorService executor = Executors.newFixedThreadPool(numberOfUsers);
        Logger.InitLogger();
        try {
            for (User user: userList) {
                executor.execute(user);
            }
        }
        finally {
            executor.shutdown();
            seatGrid.printGridState();
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