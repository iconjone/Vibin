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

var mdns = require('multicast-dns')()

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

  const [serverState, setServerState] = React.useState('Connecting...');


  var ws = React.useRef(new WebSocket('ws://vibinchair.local/ws')).current;

    // lets query for an A record for 'brunhilde.local'
    mdns.query({
      questions:[{
        name: 'vibinchair.local',
        type: 'A'
      }]
    })
  React.useEffect(() => {
    const serverMessagesList = [];
    ws.onopen = () => {
      setServerState('Connected to the server')
      ws.send("hello");
      //ws.close();
      
    };
    ws.onclose = (e) => {
      setServerState('Disconnected. Check internet or server.')
    };
    ws.onerror = (e) => {
      setServerState(e.message);
    };
    ws.onmessage = (e) => {
      // ws.send("pong");
      serverMessagesList.push(e.data);
      console.log(e);
      setServerState(e.data);
      //setServerMessages([...serverMessagesList])
    };

    // mdns.on('response', function(response) {
    //   console.log('got a response packet:', response)
    //   //find the thing by this...
    //   // response.answers.forEach(function(a) {
    //   //   console.log('  answer: %s', a.data)
    //   // }
    // })
    
    // mdns.on('query', function(query) {
    //   console.log('got a query packet:', query)
    // })
    

  }, [])
  const submitMessage = () => {
    ws.send("test");
    console.log("test?")
  
  }
  return (
    <ApplicationProvider {...eva} theme={{...eva.dark, ...theme}}>
      <HomeScreen/>
      
     <Button category="h1" onPress={()=>{submitMessage();}}>{serverState} </Button> 
    </ApplicationProvider>
  );
}

