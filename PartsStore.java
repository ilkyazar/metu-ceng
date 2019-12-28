import java.util.Scanner;
import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.util.ArrayList;
import java.io.IOException;
import java.util.stream.*; 
import java.util.List;

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

  }

  public void UpdateStock(){

  }

  public void FindCheapestMemory(int capacity){

  }

  public void FindFastestCPU(){

  }
}
