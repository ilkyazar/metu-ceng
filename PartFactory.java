import java.util.Scanner;

public class PartFactory {
    public static Part getPart (String line) {
        Scanner scanner = new Scanner(line);
        scanner.useDelimiter(",");
        int index = 0;

        Part newPart = null;
        String type = null;
        String brand = null;
        String model = null;
        String thirdProp = null;
        String fourthProp = null;
        String fifthProp = null;
        String sixthProp = null;

        while (scanner.hasNext()) {
            String property = scanner.next();
            if (index == 0) {
                type = property;
            }
            else if (index == 1) {
                brand = property;
            }
            else if (index == 2) {
                model = property;
            }
            else if (index == 3) {
                thirdProp = property;
            }
            else if (index == 4) {
                fourthProp = property;
            }
            else if (index == 5) {
                fifthProp = property;
            }
            else if (index == 6) {
                sixthProp = property;
            }
            else {
                System.out.println("Invalid PC part.");
            }
            index++;
        }

        scanner.close();

        if ("Hard Drive".equals(type)) {
            HardDrive hd = new HardDrive();
            hd.setCapacity(Integer.parseInt(thirdProp.split("GB")[0]));
            hd.setPrice(Float.parseFloat(fourthProp.split("USD")[0]));
            newPart = hd;
        }
        else if ("Monitor".equals(type)) {
            Monitor mo = new Monitor();
            mo.setAspectRatio(thirdProp);
            mo.setSize(Float.parseFloat(fourthProp));
            mo.setPrice(Float.parseFloat(fifthProp.split("USD")[0]));
            newPart = mo;
        }
        else if("PSU".equals(type)) {
            PSU psu = new PSU();
            psu.setFormFactor(thirdProp);
            psu.setWattage(Integer.parseInt(fourthProp));
            psu.setPrice(Float.parseFloat(fifthProp.split("USD")[0]));
            newPart = psu;
        }
        else if("Motherboard".equals(type)) {
            Motherboard mb = new Motherboard();
            mb.setSocket(thirdProp);
            mb.setRamSlots(Integer.parseInt(fourthProp));
            mb.setPrice(Float.parseFloat(fifthProp.split("USD")[0]));
            newPart = mb;
        }
        else if("CPU".equals(type)) {
            CPU cpu = new CPU();
            cpu.setCoreCount(Integer.parseInt(thirdProp));
            cpu.setClockSpeed(Float.parseFloat(fourthProp.split("GHz")[0]));
            cpu.setPrice(Float.parseFloat(fifthProp.split("USD")[0]));
            newPart = cpu;
        }
        else if("GPU".equals(type)) {
            GPU gpu = new GPU();
            gpu.setChipset(thirdProp);
            gpu.setCapacity(Integer.parseInt(fourthProp.split("GB")[0]));
            gpu.setClockSpeed(Float.parseFloat(fifthProp.split("MHz")[0]));
            gpu.setPrice(Float.parseFloat(sixthProp.split("USD")[0]));
            newPart = gpu;
        }
        else if("Mouse".equals(type)) {
            Mouse mouse = new Mouse();
            mouse.setConnectionType(thirdProp);
            mouse.setPrice(Float.parseFloat(fourthProp.split("USD")[0]));
            newPart = mouse;
        }
        else if("Memory".equals(type)) {
            Memory memo = new Memory();
            memo.setSocket(thirdProp);
            memo.setCapacity(Integer.parseInt(fourthProp.split("GB")[0]));
            memo.setClockSpeed(Float.parseFloat(fifthProp.split("MHz")[0]));
            memo.setPrice(Float.parseFloat(sixthProp.split("USD")[0]));
            newPart = memo;
        }
        else {
            Keyboard kb = new Keyboard();
            kb.setWired(thirdProp);
            kb.setPrice(Float.parseFloat(fourthProp.split("USD")[0]));
            newPart = kb;
        }
        
        newPart.setBrand(brand);
        newPart.setModel(model);

        return newPart;
    }
}