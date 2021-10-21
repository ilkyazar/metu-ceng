import java.io.File;
import java.util.ArrayList;
import javax.xml.parsers.DocumentBuilderFactory;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

public class SessionParser {

  public Session parse(String filename) {
    try {
      File xmlFile = new File(filename);
      Document doc = DocumentBuilderFactory.newInstance().newDocumentBuilder().parse(xmlFile);
      Element rootNode = doc.getDocumentElement();
      rootNode.normalize();
      // Session
      Session session = new Session();
      session.setTotalLaps(Integer.parseInt(rootNode.getAttribute("laps")));

      // Track
      Track track = new Track();
      Element trackNode = (Element) doc.getElementsByTagName("track").item(0);
      track.setClockwise(Boolean.parseBoolean(trackNode.getAttribute("clockwise")));
      track.setTrackName(trackNode.getAttribute("name"));

      // Track Features
      NodeList featureList = trackNode.getChildNodes();
      ArrayList<TrackFeature> trackFeatures = new ArrayList<>();
      for (int i = 0, j = 0; i < featureList.getLength(); i++) {
        Node n = featureList.item(i);
        if (featureList.item(i).getNodeType() == Node.ELEMENT_NODE) {
          Element f = (Element) n;
          double roughness = Double.parseDouble(f.getAttribute("roughness"));
          double distance = Double.parseDouble(f.getAttribute("distance"));
          TurnDirection direction = TurnDirection.valueOf(f.getAttribute("direction"));
          String type = f.getAttribute("type");
          Class[] constructor_args = new Class[4];
          constructor_args[0] = int.class;
          constructor_args[1] = TurnDirection.class;
          constructor_args[2] = double.class;
          constructor_args[3] = double.class;
          trackFeatures.add(
              (TrackFeature) Class.forName(type).getDeclaredConstructor(constructor_args)
                  .newInstance(j, direction, distance, roughness));
          j++;
        }
      }
      track.setFeatureList(trackFeatures);
      session.setTrack(track);
      // Teams
      Element teamsNode = (Element) doc.getElementsByTagName("teams").item(0);
      NodeList teamNodeList = teamsNode.getChildNodes();
      ArrayList<Team> teams = new ArrayList<>();
      for (int i = 0; i < teamNodeList.getLength(); i++) {
        Node n = teamNodeList.item(i);
        if (n.getNodeType() == Node.ELEMENT_NODE) {
          Element teamNode = (Element) n;
          String teamName = teamNode.getAttribute("name");
          String[] teamColors = teamNode.getAttribute("colors").split(";");
          // Car Info
          ArrayList<Car> cars = new ArrayList<>();
          NodeList carNodeList = teamNode.getChildNodes();
          for (int j = 0; j < carNodeList.getLength(); j++) {
            Node n1 = carNodeList.item(j);
            if (n1.getNodeType() == Node.ELEMENT_NODE) {
              Element carNode = (Element) n1;
              Car car = new Car();
              car.setCarNo(Integer.parseInt(carNode.getAttribute("no")));
              car.setDriverName(carNode.getAttribute("name"));
              car.setTire(
                  (Tire) Class.forName(carNode.getAttribute("tire")).getDeclaredConstructor()
                      .newInstance());
              cars.add(car);
            }
          }
          Team team = new Team(teamName, teamColors, cars);
          teams.add(team);
        }
      }
      session.setTeamList(teams);
      return session;
    } catch (Exception e) { // DON'T DO THIS, THIS IS NOT GOOD CODE
      e.printStackTrace();
    }
    return null;
  }
}
