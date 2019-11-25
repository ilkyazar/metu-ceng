import java.util.ArrayList;
import java.lang.*;
import java.util.Random;

public class User implements Runnable {
    private String userName;
    private ArrayList<String> wantedSeats;
    private Grid grid;

    public User() {
        this.userName = "";
        this.wantedSeats = null;
        this.grid = null;
    }

    public User(String userName, ArrayList<String> stringOfWantedSeats, Grid grid) {
        this.userName = userName;
        this.wantedSeats = stringOfWantedSeats;
        this.grid = grid;
    }

    public void setUserName(String userName) {
        this.userName = userName;
    }

    public String getUserName() {
        return this.userName;
    }

    public void setWantedSeats(ArrayList<String> wantedSeats) {
        this.wantedSeats = wantedSeats;
    }

    public ArrayList<String> getWantedSeats() {
        return this.wantedSeats;
    }

    public String getWantedSeatsString() {
        String seats = "[";
        int count = 0;
        for (String seatNumber: this.wantedSeats) {
            seats += seatNumber;
            if (count != this.wantedSeats.size()) {
                seats += ", ";
            }
        }
        seats += "]";
        return seats;
    }

    public void setGrid(Grid grid) {
        this.grid = grid;
    }

    public Grid getGrid() {
        return this.grid;
    }


    public void printUser() {
        System.out.println("Name of the user: " + this.getUserName() + ", seats: " + this.getWantedSeats());
    }

    public Boolean shouldRetry() {
        Boolean allSeatsTaken = true;

        for (String seatNumber: this.wantedSeats) {
            // If one of the seats is not taken, set allSeatsTaken to false
            Seat seat = this.getGrid().getSeatInGrid(seatNumber);
            allSeatsTaken = seat.isTaken() && allSeatsTaken;            
        }

        if (allSeatsTaken == false) {
            return true;
        }
        else {
            return false;
        }
    }

    public Boolean allWantedSeatsEmpty() {
        Boolean allWantedSeatsEmpty = true;

        for (String seatNumber: this.wantedSeats) {
            // If one of the seats is taken, set allWantedSeatsEmpty to false
            Seat seat = this.getGrid().getSeatInGrid(seatNumber);
            if (seat.isTaken()) {
                allWantedSeatsEmpty = false;
            }            
        }

        return allWantedSeatsEmpty;
    }

    public Boolean databaseFailed() {
        Random random = new Random();
        //may fail with a 10% chance
        return (random.nextInt(10) < 1) ? true : false;
    }

    @Override
    public void run() {
        // System.out.println("Inside : " + Thread.currentThread().getName());
        int tryNo = 1;
        while (this.shouldRetry()) {
            if (this.allWantedSeatsEmpty()) {
                if (this.databaseFailed() == false) {

                    for (String seatNumber: this.wantedSeats) {
                        Seat seat = this.getGrid().getSeatInGrid(seatNumber);
                        if (seat.isTaken() == false)
                            seat.reserve(this);
                        else
                            break; 
                    }

                    try {
                        Thread.sleep(50);
                    }
                    catch (InterruptedException exception) {
                        exception.printStackTrace();
                    }
                    Logger.LogSuccessfulReservation(this.getUserName(), this.getWantedSeatsString(), System.nanoTime(), "Reservation successful. Retry No: " + tryNo);
                
                }
                else {
                    Logger.LogDatabaseFailiure(this.getUserName(), this.getWantedSeatsString(), System.nanoTime(), "Database failure. Trying again. ");
                    try {
                        Thread.sleep(100);
                    }
                    catch (InterruptedException exception) {
                        exception.printStackTrace();
                    }
                }
            }
            else {
                Logger.LogFailedReservation(this.getUserName(), this.getWantedSeatsString(), System.nanoTime(), "Reservation failed. Seats are not available.");
            }
            tryNo++;
        }

    }
}
