public class Motherboard extends Part {
    private int ramSlots;
    private String socket;

    public Motherboard() {
        super();
    }

    public int getRamSlots() {
        return this.ramSlots;
    }

    public String getSocket() {
        return this.socket;
    }

    public void setRamSlots(int ramSlots) {
        this.ramSlots = ramSlots;
    }

    public void setSocket(String socket) {
        this.socket = socket;
    }

    @Override
	public String toString(){
		return "\nBrand="+getBrand()+"\tModel="+getModel()+"\tPrice="+String.valueOf(getPrice())+"\tSocket="+getSocket()+"\tRamSlots="+String.valueOf(getRamSlots());
	}

}