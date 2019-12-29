public class TestMain {
    public static void main(String[] args) {
        PartsStore ps = new PartsStore();
        //ps.FindPartsWithBrand("GPU", "Asus");
        //ps.TotalPrice("GPU", "Asus", "GeForce RTX 2080");
        //ps.TotalPrice(null, "Asus", null);
        //ps.TotalPrice(null, null, null);
        ps.FindCheapestMemory(16);
        //ps.FindFastestCPU();
    }
}