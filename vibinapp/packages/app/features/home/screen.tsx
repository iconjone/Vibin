import { Anchor, Button, H1, Paragraph, Separator, Sheet, XStack, YStack, XGroup, Switch, Image, Label } from '@my/ui'
import { ChevronDown, ChevronUp, Divide, Feather, PartyPopper, Flower } from '@tamagui/lucide-icons'
import React, { useState } from 'react'
import { useLink } from 'solito/link'

import {SheetDemo} from './sheet'

export function HomeScreen( {vibinchair, vibinchairStatus}) {
  const linkProps = useLink({
    href: '/user/Jonathan',
  })

  const [enabled, setEnabled] = useState(true);



  //function to send message to websocket
  const send = (message) => {
    //maybe do a check to see if the socket is open & message would work
    vibinchair.send(JSON.stringify(message));
    //heyyyyyy cutie IM IN YOUR CODE NOW
  }

  const [levelMode, setLevelMode] = useState("strong");

  const strongSet = () =>{
    setLevelMode("strong");
    vibinchair.controlMasterVol(1023);
    vibinchair.controlVol("amp1", "LM",1023);
    vibinchair.controlVol("amp1", "RM",1023);
    vibinchair.controlVol("amp2", "LM",1023);
    vibinchair.controlVol("amp2", "RM",1023);
    vibinchair.controlVol("amp3", "LM",1023);
    vibinchair.controlVol("amp3", "RM",1023);
  }

  
  const comfortSet = () =>{
    setLevelMode("comfort");
    vibinchair.controlMasterVol(750);
    vibinchair.controlVol("amp1", "LM",750);
    vibinchair.controlVol("amp1", "RM",750);
    vibinchair.controlVol("amp2", "LM",750);
    vibinchair.controlVol("amp2", "RM",750);
    vibinchair.controlVol("amp3", "LM",750);
    vibinchair.controlVol("amp3", "RM",750);
  }

    
  const lightSet = () =>{
    setLevelMode("light");
    vibinchair.controlMasterVol(750);
    vibinchair.controlVol("amp1", "LM",450);
    vibinchair.controlVol("amp1", "RM",450);
    vibinchair.controlVol("amp2", "LM",450);
    vibinchair.controlVol("amp2", "RM",450);
    vibinchair.controlVol("amp3", "LM",450);
    vibinchair.controlVol("amp3", "RM",450);
  }



  
  return (
    <YStack f={1} jc="center" ai="center" p="$4" backgroundColor="#2a3854" space>
      <YStack space="$4" maw={600}>
       {vibinchairStatus && <H1 ta="center">Welcome to Vibin.</H1>}
        {/* <Paragraph ta="center">
          Connect to Vibin to get started.
        </Paragraph> */}
      </YStack>

      
      
      <XGroup size="$12">
      <Button icon={Flower} w="32%" theme={levelMode ==="light" ?"blue":"alt2"} onPress={lightSet}>
        Light
      </Button>
      <Button icon={Feather} w="33%" theme={levelMode ==="comfort" ?"blue":"alt2"} onPress={comfortSet}>
        Comfort
      </Button>
        <Button icon={PartyPopper} w="32%" theme={levelMode ==="strong" ?"blue":"alt2"} onPress={strongSet}>
        Strong
      </Button>
      </XGroup>
      <XStack w={200} ai="center" space="$4">
      <Label pr="$0" miw={90} jc="flex-end"  size="$8">
        {enabled ? "Enabled" : "Disabled"}
      </Label>
      <Separator mih={40} vertical  size="$8"/>
      <Switch size="$8"  defaultChecked = {true} onCheckedChange={(check)=>{setEnabled(check)}}>
    <Switch.Thumb animation="bouncy" />
  </Switch>
  </XStack>

      <SheetDemo vibinchair={vibinchair} setLevelMode={setLevelMode}/>

      <XGroup size="$3">
        <Button {...linkProps} theme="light">DSP Settings</Button>
      </XGroup>


    </YStack>
  )
}


