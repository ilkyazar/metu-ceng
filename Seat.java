public class Seat {
    private Boolean taken;
    private String seatNumber;

    public Seat() {
        this.taken = false;
        this.seatNumber = "";
    }

    public Seat(String seatNumber) {
        this.taken = false;
        this.seatNumber = seatNumber;
    }

    public String getSeatNumber() {
        return this.seatNumber;
    }

    public void setTaken() {
        this.taken = true;
    }

    public Boolean isTaken() {
        return this.taken;
    }
}
