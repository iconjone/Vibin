import { StatusBar } from "expo-status-bar";
import { StyleSheet, View, SafeAreaView } from "react-native";
import React from "react";
import * as eva from "@eva-design/eva";
import { default as theme } from './theme.json';
import {
  ApplicationProvider,
  Layout,
  Text,
  Toggle,
  Button
} from "@ui-kitten/components";

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

const HomeScreen = () => (
  <Layout style={{ flex: 1, justifyContent: "center", alignItems: "center" }}>
    <Text category="h1"> Connect to Vibin</Text>
    
 

    <ToggleSimpleUsageShowcase />
  </Layout>
);

export default function App() {

  const [serverState, setServerState] = React.useState('Loading...');


  var ws = React.useRef(new WebSocket('ws://vibinchair.local/ws')).current;


  React.useEffect(() => {
    const serverMessagesList = [];
    ws.onopen = () => {
      setServerState('Connected to the server')
    };
    ws.onclose = (e) => {
      setServerState('Disconnected. Check internet or server.')
    };
    ws.onerror = (e) => {
      setServerState(e.message);
    };
    ws.onmessage = (e) => {
      serverMessagesList.push(e.data);
      console.log(e);
      setServerState(e.data);
      //setServerMessages([...serverMessagesList])
    };
  }, [])
  const submitMessage = () => {
    ws.send(messageText);
    setMessageText('')
    setInputFieldEmpty(true)
  }
  return (
    <ApplicationProvider {...eva} theme={{...eva.dark, ...theme}}>
      <HomeScreen/>
      
     <Button category="h1">{serverState} </Button> 
    </ApplicationProvider>
  );
}

