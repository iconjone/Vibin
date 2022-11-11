'use strict';
import React from "react";
import {
    Text,
    Toggle,
    Button,

  } from "@ui-kitten/components";

  // import {Slider} from "./Slider.js"

  import { NativeBaseProvider, Slider, Box } from "native-base";


  
  const Status = ({ vibinchair, serverState }) => {
    const [enableState, setToggleState] = React.useState(false);
    const [muteState, setToggleState1] = React.useState(false);
    const [resetState, setToggleState2] = React.useState(false);



    const reconnect = () => {
      console.log("reconnecting", vibinchair);
      vibinchair.current.init();
    };
    const onCheckedChange = (isChecked) => {
        setToggleState(isChecked);
        if(isChecked)
         vibinchair.current.send({type: "command", origin: "app", nId:0, data: "en-on"});
           else
           vibinchair.current.send({type: "command", origin: "app", nId:0, data: "en-off"});
      };
        const onCheckedChange1 = (isChecked) => {
        setToggleState1(isChecked);
        if(isChecked)
            vibinchair.current.send({type: "command", origin: "app", nId:0, data: "m-on"});
              else
                vibinchair.current.send({type: "command", origin: "app", nId:0, data: "m-off"});
        };
        const onCheckedChange2 = (isChecked) => {
            setToggleState2(isChecked);
            if(isChecked)
                vibinchair.current.send({type: "command", origin: "app", nId:0, data: "r-on"});
                  else
                    vibinchair.current.send({type: "command", origin: "app", nId:0, data: "r-off"});
            };
    return (
      <NativeBaseProvider>
        <Button
          category="h1"
          onPress={reconnect}
          status={serverState ? "success" : "danger"}
        >
          {serverState ? "Connected" : "Connecting..."}
        </Button>
        <Text category="h1"> Welcome to Vibin {vibinchair.status}</Text>
  
        <Toggle checked={enableState} onChange={onCheckedChange} status='success'>
        Enable
      </Toggle>
      <Toggle checked={muteState} onChange={onCheckedChange1} status ='danger'>
        Mute
      </Toggle>

      <Toggle checked={resetState} onChange={onCheckedChange2} status='warning'>
        Reset
      </Toggle>


      <Box alignItems="center" w="50%" >
      <Slider  maxW="300" defaultValue={70} minValue={0} maxValue={1000} size="lg">
        <Slider.Track>
          <Slider.FilledTrack />
        </Slider.Track>
        <Slider.Thumb />
      </Slider>
      </Box>
   


      </NativeBaseProvider>
    );
  };

  // const styles = StyleSheet.create({
  //   container: {
  //     flex: 1,
  //     marginLeft: 10,
  //     marginRight: 10,
  //     alignItems: "stretch",
  //     justifyContent: "center"
  //   }
  // });

  

  export {Status};