import java.util.ArrayList;

public class Grid {
    private int n;
    private int m;
    private ArrayList<ArrayList<Seat>> seatGrid;

    public Grid() {
        this.n = 0;
        this.m = 0;
        this.seatGrid = null;
    }

    public Grid(int n, int m) {
        this.n = n;
        this.m = m;
        this.seatGrid = new ArrayList<ArrayList<Seat>>(this.n);

        for (int row = 0; row < this.n; row++) {
            ArrayList<Seat> columnList = new ArrayList<Seat>(m);
            
            for (int col = 0; col < this.m; col++) {
                Seat emptySeat = new Seat();
                emptySeat.setSeatNumber(row, col);
                columnList.add(emptySeat);
            }
            seatGrid.add(columnList);
            
        }

    }

    public int getRowCount() {
        return this.n;
    }

    public int getColumnCount() {
        return this.m;
    }

    public Seat getSeatInGrid(int r, int c) {
        return this.seatGrid.get(r).get(c);
    }

    public Seat getSeatInGrid(String seatNumber) {
        char letter = seatNumber.charAt(0);
        char digit = seatNumber.charAt(1);
        int r = letter - 65;
        int c = digit - '0';
        return getSeatInGrid(r, c);
    }

    public void printGrid() {
        for (int row = 0; row < this.n; row++) {            
            for (int col = 0; col < this.m; col++) {
                System.out.print(this.getSeatInGrid(row, col).getSeatNumber() + " "); 
            }
            System.out.println();
        }
    }

    public void printGridState() {
        for (int row = 0; row < this.n; row++) {            
            for (int col = 0; col < this.m; col++) {

                String seatStatus;
                Seat seat = this.getSeatInGrid(row, col);
                if (seat.isTaken()) {
                    seatStatus = "T:" + seat.getReservedUser().getUserName();
                }
                else {
                    seatStatus = "E:     ";
                }

                System.out.print(seatStatus + " "); 
            }
            System.out.println();
        }
    }



}
