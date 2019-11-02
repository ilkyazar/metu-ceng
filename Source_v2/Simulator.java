public class Simulator {

  public static void main(String[] args) {
    SessionParser parser = new SessionParser();
    Session s = parser.parse("spa.xml");
    s.simulate();
  }
}
