import java.util.Scanner;
import java.util.ArrayList;

public class Main {
    public static void main(String[] args) {
        Scanner in = new Scanner(System.in); 
  
        int n = in.nextInt(); 
        int m = in.nextInt(); 
        int numberOfUsers = in.nextInt(); 
  
        in.nextLine();

        ArrayList<User> userList = createUserList(in, numberOfUsers);

        Grid seatGrid = new Grid(n, m);
        seatGrid.printGrid();
        seatGrid.printGridState();
        printUserList(userList);
    }

    public static ArrayList<User> createUserList(Scanner in, int numberOfUsers) {
        ArrayList<User> userList = new ArrayList<User>();

        for (int i = 0; i < numberOfUsers; i++) {
            String li = in.nextLine();

            String[] tokens = li.split("[ ]");

            User user = new User();
            ArrayList<Seat> userSeats = new ArrayList<Seat>();

            String userName = tokens[0];
            user.setUserName(userName);

            int numberOfSeats = tokens.length - 1;

            for (int j = 1; j < numberOfSeats + 1; j++) {
                Seat seat = new Seat(tokens[j]);
                userSeats.add(seat);
            }

            user.setlistOfWantedSeats(userSeats);
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