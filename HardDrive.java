public class HardDrive extends Part {
    private int capacity;

    public HardDrive() {
        super();
    }

    public int getCapacity() {
        return this.capacity;
    }

    public void setCapacity(int capacity) {
        this.capacity = capacity;
    }

    @Override
	public String toString(){
		return "\nBrand="+getBrand()+"\tModel="+getModel()+"\tPrice="+String.valueOf(getPrice())+"\tCapacity="+String.valueOf(getCapacity());
	}

}