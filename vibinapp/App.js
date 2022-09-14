import { StatusBar } from "expo-status-bar";
import {
  StyleSheet,
  View,
  SafeAreaView,
  Platform,
  Dimensions,
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
  ButtonGroup,
  TopNavigation,
  TopNavigationAction,
  Icon,
  TabView,
  Tab,
} from "@ui-kitten/components";

import { vibinchairControl } from "./vibinConnection";

import { Status } from "./Status";

import { HeartRate } from "./HeartRate";

// var mdns = require('multicast-dns')()

export default function App() {
  const [serverState, setServerState] = React.useState(false);
  const [selectedIndex, setSelectedIndex] = React.useState(1);
  const vibinchair = React.useRef(null);
  const [heartRate, setHeartRate] = React.useState([95]);
  const [voltage, setVoltage] = React.useState(0);
  const [current, setCurrent] = React.useState(0);
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

  return (
    <ApplicationProvider {...eva} theme={{ ...eva.dark, ...theme }}>
      <Layout level="1">
        <TopNavigation
          title="Vibin"
          alignment="center"
          style={{ marginTop: Platform.OS === "ios" ? 40 : 0 }}
        />
        <TabView
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
              <HeartRate alignment="center" heartRate={heartRate} />
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
        </TabView>
      </Layout>
    </ApplicationProvider>
  );
}
