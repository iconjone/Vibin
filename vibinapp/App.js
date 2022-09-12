import { StatusBar } from "expo-status-bar";
import { StyleSheet, View, SafeAreaView, Platform } from "react-native";
import React from "react";
import * as eva from "@eva-design/eva";
import { default as theme } from "./theme.json";
import {
  ApplicationProvider,
  Layout,
  Text,
  Toggle,
  Button,
  TopNavigation,
  TopNavigationAction,
  Icon,
  TabView,
  Tab,
} from "@ui-kitten/components";

import { vibinchairControl } from "./vibinConnection";

import { Status } from "./Status";


// var mdns = require('multicast-dns')()




export default function App() {
  const [serverState, setServerState] = React.useState(false);
  const [selectedIndex, setSelectedIndex] = React.useState(1);
  const vibinchair = React.useRef(null);
  //vibinchair = vibinchair.current;

  React.useEffect(() => {
    vibinchair.current = new vibinchairControl();
    console.log(vibinchair.current);
    vibinchair.current.socket.onopen = () => {
      setServerState(true);
      console.log("connected to server");
    }
    //on a message update whatever data is needed
  }, []);



  return (
    <ApplicationProvider {...eva} theme={{ ...eva.dark, ...theme }}>
       <Layout level="1" >
      
      <TopNavigation title="Vibin" alignment="center" style={{marginTop: Platform.OS === 'ios' ? 40 : 0}}/>
      <TabView
        selectedIndex={selectedIndex}
        onSelect={(index) => setSelectedIndex(index)}
        indicatorStyle={{color:'color-danger-300', backgroundColor:'color-danger-300', fontWeight:'bold'}}
      >
        <Tab title="USERS">
          <Layout style={{ height:1600}}>
            <Text category="h5">ORDERS</Text>

          </Layout>
        </Tab>
        <Tab title="Status" >
          
          <Layout style={{ height:1600}}>
            <Status serverState={serverState} vibinchair={vibinchair} />
          </Layout>
        </Tab>
        <Tab title="HRV">
          <Layout style={{ height:1600}}>
            <Text category="h1">HRV</Text>
          </Layout>
        </Tab>
      </TabView>
      </Layout>
    </ApplicationProvider>
  );
}
