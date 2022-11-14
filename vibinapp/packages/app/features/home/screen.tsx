import { Anchor, Button, H1, Paragraph, Separator, Sheet, XStack, YStack, XGroup, Switch, Image, Label } from '@my/ui'
import { ChevronDown, ChevronUp, Divide, Feather, PartyPopper } from '@tamagui/lucide-icons'
import React, { useState } from 'react'
import { useLink } from 'solito/link'

import {SheetDemo} from './sheet'

export function HomeScreen( {vibinchair, vibinchairStatus}) {
  const linkProps = useLink({
    href: '/user/Jonathan',
  })



  //function to send message to websocket
  const send = (message) => {
    //maybe do a check to see if the socket is open & message would work
    vibinchair.send(JSON.stringify(message));
    //heyyyyyy cutie IM IN YOUR CODE NOW
  }

  const strongSet = () =>{
    vibinchair.controlMasterVol(1023);
    vibinchair.controlVol("amp1", "LM",1023);
    vibinchair.controlVol("amp1", "RM",1023);
    vibinchair.controlVol("amp2", "LM",1023);
    vibinchair.controlVol("amp2", "RM",1023);
    vibinchair.controlVol("amp3", "LM",1023);
    vibinchair.controlVol("amp3", "RM",1023);
  }

  
  const comfortSet = () =>{
    vibinchair.controlMasterVol(750);
    vibinchair.controlVol("amp1", "LM",750);
    vibinchair.controlVol("amp1", "RM",750);
    vibinchair.controlVol("amp2", "LM",750);
    vibinchair.controlVol("amp2", "RM",750);
    vibinchair.controlVol("amp3", "LM",750);
    vibinchair.controlVol("amp3", "RM",750);
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
      <Button icon={Feather} w="50%" theme="alt2" onPress={comfortSet}>
        Comfort
      </Button>
        <Button icon={PartyPopper} w="50%" theme="alt1" onPress={strongSet}>
        Strong
      </Button>
      </XGroup>
      <XStack w={200} ai="center" space="$4">
      <Label pr="$0" miw={90} jc="flex-end"  size="$8">
        Enabled
      </Label>
      <Separator mih={40} vertical  size="$8"/>
      <Switch size="$8" theme="light">
    <Switch.Thumb animation="bouncy" />
  </Switch>
  </XStack>

      <SheetDemo vibinchair={vibinchair}/>

      <XGroup size="$3">
        <Button {...linkProps} theme="light">DSP Settings</Button>
      </XGroup>


    </YStack>
  )
}


