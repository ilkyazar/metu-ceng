public class PSU extends Part {
    private int wattage;
    private String formFactor;

    public PSU() {
        super();
    }

    public int getWattage() {
        return this.wattage;
    }

    public String getFormFactor() {
        return this.formFactor;
    }

    public void setWattage(int wattage) {
        this.wattage = wattage;
    }

    public void setFormFactor(String formFactor) {
        this.formFactor = formFactor;
    }

    @Override
	public String toString(){
		return "\nBrand="+getBrand()+"\tModel="+getModel()+"\tPrice="+String.valueOf(getPrice())+"\tWattage="+String.valueOf(getWattage())+"\tFormFactor="+getFormFactor();
	}

}