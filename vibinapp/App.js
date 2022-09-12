import { StatusBar } from "expo-status-bar";
import { StyleSheet, View, SafeAreaView } from "react-native";
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
} from "@ui-kitten/components";
import { vibinchairControl } from "./vibinConnection";

// var mdns = require('multicast-dns')()



const ToggleSimpleUsageShowcase = () => {
  const [checked, setChecked] = React.useState(false);

  const onCheckedChange = (isChecked) => {
    setChecked(isChecked);
  };

  return (
    <Toggle checked={checked} onChange={onCheckedChange}>
      {`Checked: ${checked}`}
    </Toggle>
  );
};

const HomeScreen = ({ vibinchair, serverState }) => {
  sendMessage = () => {
    console.log("sending message");
    vibinchair.send("test yeah");
  };
  return (
    <div>
      <TopNavigation
        title="Vibin"
        alignment="center"
      />
      <Button category="h1" onPress={sendMessage}>hi {serverState} </Button>
      <Text category="h1"> Welcome to Vibin {vibinchair.status}</Text>


      <ToggleSimpleUsageShowcase />
    </div>
  );
};

export default function App() {
  const [serverState, setServerState] = React.useState("Connecting...");
  //console.log(vibinchairControl)
  // var vibinchair = React.useRef(new vibinchairControl()).current;
  // console.log(vibinchair.init)
  // console.log(vibinchair.status)
  var vibinchair = React.useRef(new vibinchairControl()).current;
  console.log(vibinchair);

  React.useEffect(() => {
    setServerState(vibinchair.status + "yeah");
  });

  return (
    <ApplicationProvider {...eva} theme={{ ...eva.dark, ...theme }}>
      <Layout>
        <HomeScreen serverState={serverState} vibinchair={vibinchair} />
      </Layout>
    </ApplicationProvider>
  );
}
