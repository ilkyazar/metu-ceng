import java.util.Scanner;
import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.util.ArrayList;
import java.io.IOException;
import java.util.stream.*; 
import java.util.List;
import java.util.Collections;
import java.util.Comparator;

public class PartsStore {
  private ArrayList<Part> pcParts;

  public PartsStore(){
    /* 
      read the csv file, process it, and store the processed data as private fields in PartsStore
      filename won't change ("pcparts.csv") 
      it will be in the same path as your .class files. So just "pcparts.csv" as file path is enough.
    */

    this.pcParts = new ArrayList<>();

    BufferedReader bufReader = null;

    try {
      //bufReader = new BufferedReader(new FileReader("pcparts-2.csv"));
      bufReader = new BufferedReader(new FileReader("pcparts.csv"));
    }
    catch(FileNotFoundException e) {
      e.printStackTrace();
    }

    String line = null;

    try {
      line = bufReader.readLine();
    }
    catch(IOException e) {
      e.printStackTrace();
    }
    
    while (line != null) {
      Part newPart = null;

      newPart = PartFactory.getPart(line);

      this.pcParts.add(newPart);

      try {
        line = bufReader.readLine();
      }
      catch(IOException e) {
        e.printStackTrace();
      }
    }

    try {
      bufReader.close();
    }
    catch(IOException e) {
      e.printStackTrace();
    }

    //System.out.println(this.pcParts);
  }

  public void FindPartsWithBrand(String type, String brand){
    List<Part> newList = null;

    if (type == null) {
      newList = this.pcParts.stream().filter(p -> p.getBrand().equals(brand)).collect(Collectors.toList());
    }

    else {
      newList = this.pcParts.stream().filter(p -> p.getType().equals(type) && p.getBrand().equals(brand))
                                                .collect(Collectors.toList());
    }
    
    for (Part p : newList) { 		      
      System.out.println(p.getCsvLine());	
    }

  }

  public void TotalPrice(String type, String brand, String model){
    double price = 0;
    price = this.pcParts.stream().filter(p -> ((type != null) ? p.getType().equals(type) : true))
                                 .filter(p -> ((brand != null) ? p.getBrand().equals(brand) : true))
                                 .filter(p -> ((model != null) ? p.getModel().equals(model) : true))
                                 .mapToDouble(p -> p.getPrice())
                                 .sum();
    
    String priceStr = String.format("%.2f", price);
    String totalPrice = priceStr + " USD";
    System.out.printf(totalPrice);
    /*
    List<Part> newList = null;
    newList = this.pcParts.stream().filter(p -> ((type != null) ? p.getType().equals(type) : true))
                                   .filter(p -> ((brand != null) ? p.getBrand().equals(brand) : true))
                                   .filter(p -> ((model != null) ? p.getModel().equals(model) : true))
                                   .collect(Collectors.toList());

    for (Part p : newList) { 		      
      System.out.println(p.getCsvLine());	
    }
    */
    
  }

  public void UpdateStock(){
    List<Part> newList = null;

    newList = this.pcParts.stream().filter(p -> p.getPrice() != 0)
                                   .collect(Collectors.toList());

    int itemsRemoved = this.pcParts.size() - newList.size();

    System.out.println(itemsRemoved + " items removed.\n");
    
    this.pcParts = new ArrayList<Part>(newList);
/*
    for (Part p : this.pcParts) { 		      
      System.out.println(p.getCsvLine());	
    }    
*/
  }

  public void FindCheapestMemory(int capacity){
    Part part = null;
    List<Part> newList = null;

    newList = this.pcParts.stream().filter(p -> p.getType().equals("Memory"))
                                   .filter(p -> ((Memory) p).getCapacity() >= capacity)
                                   .filter(p -> p.getPrice() != 0)
                                   .collect(Collectors.toList());

    part =  Collections.min(newList, Comparator.comparing(p -> p.getPrice()));

    System.out.println(part.getCsvLine());	

/*
    for (Part p : newList) { 		      
      System.out.println(p.getCsvLine());	
      System.out.println(p.getPrice());	
    }
*/
  }

  public void FindFastestCPU(){
    Part part = null;
    List<Part> newList = null;

    newList = this.pcParts.stream().filter(p -> p.getType().equals("CPU"))
                                   .filter(p -> p.getPrice() != 0)
                                   .collect(Collectors.toList());
                          
    part =  Collections.max(newList, Comparator.comparing(p -> ((CPU) p).getCoreCount() * ((CPU) p).getClockSpeed()));

    System.out.println(part.getCsvLine());	
/*
    for (Part p : newList) { 		      
      System.out.println(p.getCsvLine());	
      System.out.println(((CPU) p).getCoreCount() * ((CPU) p).getClockSpeed());	
    }
*/
  }
}
