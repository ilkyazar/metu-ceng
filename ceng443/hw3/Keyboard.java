public class Keyboard extends Part {
    private String wired;

    public Keyboard() {
        super();
    }

    public String getWired() {
        return this.wired;
    }

    public void setWired(String wired) {
        this.wired = wired;
    }

    @Override
	public String toString(){
		return "\nBrand="+getBrand()+"\tModel="+getModel()+"\tPrice="+String.valueOf(getPrice())+"\tWired="+getWired();
	}

}