import java.util.ArrayList;
import java.util.Random;

public class User implements Runnable {
    private String userName;
    private ArrayList<String> wantedSeats;
    private Grid grid;
    private String notAvailableComment = "";
    
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
        int count = 1;
        for (String seatNumber: this.wantedSeats) {
            seats += seatNumber;
            if (count != this.wantedSeats.size()) {
                seats += ", ";
            }
            count++;
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
        System.out.println("Name of the user: " + this.getUserName() + ", seats: " + this.getWantedSeatsString());
    }

    public Boolean shouldRetry(int tryNo) {
        Boolean allSeatsTaken = true;

        for (String seatNumber: this.wantedSeats) {
            // If one of the seats is not taken, set allSeatsTaken to false
            Seat seat = this.getGrid().getSeatInGrid(seatNumber);
            allSeatsTaken = seat.isTaken() && allSeatsTaken;   
        }

        if (allSeatsTaken == false || tryNo == 1) {
            return true;
        }
        else {
            return false;
        }
    }

    public Boolean allWantedSeatsEmpty() {
        Boolean allWantedSeatsEmpty = true;

        for (String seatNumber: this.wantedSeats) {
            // If one of the seats is taken by somebody else, set allWantedSeatsEmpty to false
            Seat seat = this.getGrid().getSeatInGrid(seatNumber);
            if (seat.isTaken()) {
                if (seat.getReservedUser().getUserName().equals(this.getUserName()) == false) {
                    allWantedSeatsEmpty = false;
                    this.notAvailableComment += (seatNumber + " is taken by " + seat.getReservedUser().getUserName() + ". ");
                }
            }            
        }

        return allWantedSeatsEmpty;
    }

    public Boolean allTaken() {
        Boolean allTaken = true;
        for (String seatNumber: this.wantedSeats) {
            Seat seat = this.getGrid().getSeatInGrid(seatNumber);
            // If the seat is reserved by someone else or it's empty, return false
            if ((seat.isTaken() && seat.getReservedUser().getUserName().equals(this.getUserName()) == false) || 
                seat.isTaken() == false) {
                allTaken = false;
            }
        }

        return allTaken;
    }

    public Boolean databaseFailed() {
        Random random = new Random();
        // Database may fail with a 10% probability
        return (random.nextInt(10) < 1) ? true : false;
    }

    @Override
    public void run() {
        // System.out.println("Inside : " + Thread.currentThread().getName());

        int tryNo = 1;
        while (this.shouldRetry(tryNo)) {
            if (this.allWantedSeatsEmpty()) {
                if (this.databaseFailed() == false) {

                    for (String seatNumber: this.wantedSeats) {
                        Seat seat = this.getGrid().getSeatInGrid(seatNumber);
                        if (seat.isTaken() == false && this.allWantedSeatsEmpty()) {
                            seat.reserve(this);
                        }
                        else {
                            // Give up seats
                            if (seat.isTaken() == true && seat.getReservedUser().getUserName().equals(this.userName) == false) {
                                for (String seatNu: this.wantedSeats) {
                                    Seat seatToGiveUp = this.getGrid().getSeatInGrid(seatNu);
                                    if (seatToGiveUp.isTaken() && seatToGiveUp.getReservedUser().getUserName().equals(this.userName)) {
                                        seatToGiveUp.setEmpty();
                                        
                                        //System.out.println(seatNu + " is set as EMPTY by " + this.getUserName() + " " + System.nanoTime());
                                    }
                                }
                            }
                            break; 
                        }
                    }

                    try {
                        Thread.sleep(50);
                    }
                    catch (InterruptedException exception) {
                        exception.printStackTrace();
                    }
                    if (this.allTaken())
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
                Logger.LogFailedReservation(this.getUserName(), this.getWantedSeatsString(), System.nanoTime(), "Reservation failed. " + this.notAvailableComment);
                break;
            }
            tryNo++;
        }
    }
}
