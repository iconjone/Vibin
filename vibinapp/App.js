import { StatusBar } from "expo-status-bar";
import {
  View,
  Platform,
  useWindowDimensions,
  StyleSheet,
} from "react-native";
import React from "react";
import * as eva from "@eva-design/eva";
import { default as theme } from "./theme.json";
import {
  ApplicationProvider,
  Layout,
  Text,
  Toggle,
  Button,
  Modal,
  ButtonGroup,
  TopNavigation,
  TopNavigationAction,
  Icon,
  Tab,
  Card
} from "@ui-kitten/components";

import { vibinchairControl } from "./vibinConnection";

import { TabView, SceneMap } from 'react-native-tab-view';
import { NativeBaseProvider, Slider } from "native-base";

// import { Status } from "./Status";

// import { HeartRate } from "./HeartRate";

// var mdns = require('multicast-dns')()

export default function App() {
  const [serverState, setServerState] = React.useState(false);
  const [selectedIndex, setSelectedIndex] = React.useState(1);
  const vibinchair = React.useRef(null);
  const [heartRate, setHeartRate] = React.useState([95]);
  const [voltage, setVoltage] = React.useState(0);
  const [current, setCurrent] = React.useState(0);

  
const FirstRoute = () => (
  <View style={{ flex: 1, backgroundColor: '#ff4081' }} >
    <NativeBaseProvider>
    <Text category="h1"> Welcome to Vibin {vibinchair.status}</Text>
   
    </NativeBaseProvider>
  </View>


);

const SecondRoute = () => (
  <View style={{ flex: 1, backgroundColor: '#673ab7' }} />
);

const renderScene = SceneMap({
  first: FirstRoute,
  // second: SecondRoute,
});
  //vibinchair = vibinchair.current;

  React.useEffect(() => {
    var counter = 0;
    vibinchair.current = new vibinchairControl();
    console.log(vibinchair.current);
    vibinchair.current.socket.onopen = () => {
      setServerState(true);
      console.log("connected to server");
    };
    vibinchair.current.socket.onclose = () => {
      setServerState(false);
      console.log("disconnected from server");
    };
    vibinchair.current.socket.onmessage = (message) => {
      // console.log("message received", message);
      const data = JSON.parse(message.data);
      console.log(data);
      if (data.data.split("-")[0] == "bpm") {
        //output the last 10 value

        setHeartRate((heartRate) => [
          ...heartRate.slice(heartRate.length > 10 ? 1 : 0),
          parseInt(data.data.split("-")[1]),
        ]);
      }
      if (data.data.split("-")[0] == "v") {
        setVoltage(parseInt(data.data.split("&")[1]));
      }
      if (data.data.split("-")[0] == "ma") {
        setCurrent(parseInt(data.data.split("&")[1]));
      }
    };
    //on a message update whatever data is needed
  }, []);
  const layout = useWindowDimensions();

  const [index, setIndex] = React.useState(0);
  const [routes] = React.useState([
    { key: 'first', title: 'First' },
    // { key: 'second', title: 'Second' },
  ]);
  const reconnect = () => {
    console.log("reconnecting", vibinchair);
    vibinchair.current.init();
  };

  const [visible, setVisible] = React.useState(false);


  return (


    <ApplicationProvider {...eva} theme={{ ...eva.dark, ...theme }}>
      <Layout style={{ flex: 1 }}>
            <TopNavigation
          title="Vibin"
          alignment="center"
          style={{ marginTop: Platform.OS === "ios" ? 40 : 0 }}
        />

   <NativeBaseProvider>
    <Button
          category="h1"
          onPress={reconnect}
          status={serverState ? "success" : "danger"}
        >
          {serverState ? "Connected" : "Connecting..."}
        </Button>
    <Slider w="3/4" maxW="300" defaultValue={70} minValue={0} maxValue={100} accessibilityLabel="hello world" step={1} size="lg">
        <Slider.Track>
          <Slider.FilledTrack />
        </Slider.Track>
        <Slider.Thumb />
      </Slider>

      <Button onPress={() => setVisible(true)}>
        TOGGLE MODAL
      </Button>

      <Modal
        visible={visible}
        backdropStyle={styles.backdrop}
        onBackdropPress={() => setVisible(false)}>
        <Card disabled={true}>
          <Text>Welcome to UI Kitten 😻</Text>
          <Slider w="3/4" maxW="300" defaultValue={70} minValue={0} maxValue={100} accessibilityLabel="hello world" step={10}>
        <Slider.Track>
          <Slider.FilledTrack />
        </Slider.Track>
        <Slider.Thumb />
      </Slider>
          <Button onPress={() => setVisible(false)}>
            DISMISS
          </Button>
        </Card>
      </Modal>

      
    </NativeBaseProvider>
    
      </Layout>
    </ApplicationProvider>
  );
}


        {/* <TabView
          selectedIndex={selectedIndex}
          onSelect={(index) => setSelectedIndex(index)}
          indicatorStyle={{
            color: "color-danger-300",
            backgroundColor: "color-danger-300",
            fontWeight: "bold",
          }}
        >
          <Tab title="BPM">
            <Layout style={{ height: Dimensions.get("window").height }}>
              {/* <HeartRate alignment="center" heartRate={heartRate} /> 
            </Layout>
          </Tab>
          <Tab title="STATUS">
            <Layout style={{ height: Dimensions.get("window").height }}>
              <Status serverState={serverState} vibinchair={vibinchair} />
            </Layout>
          </Tab>
          <Tab title="POWER" color="warning">
            <Layout style={{ height: Dimensions.get("window").height }}>
              <Text category="h1" alignment="center">
                {" "}
                Power
              </Text>
              <Layout styles={{alignItems: 'center'}}>
              <ButtonGroup appearance="outline" status="primary" size='giant' alignment='center'>
                <Button
                  size="giant"
                  width={Dimensions.get("window").width / 2 - 10}
                >
                  {" "}
                  Voltage: {voltage / 1000} V
                </Button>
                <Button
                  size="giant"
                  width={Dimensions.get("window").width / 2 - 10}
                >
                  Current: {current} mA
                </Button>
              </ButtonGroup>
              </Layout>
            </Layout>
          </Tab>
        </TabView> */}


        const styles = StyleSheet.create({
          container: {
            minHeight: 192,
          },
          backdrop: {
            backgroundColor: 'rgba(0, 0, 0, 0.5)',
          },
        });