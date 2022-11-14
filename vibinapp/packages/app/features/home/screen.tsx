import { Anchor, Button, H1, Paragraph, Separator, Sheet, XStack, YStack, Slider, Image } from '@my/ui'
import { ChevronDown, ChevronUp, Divide } from '@tamagui/lucide-icons'
import React, { useState } from 'react'
import { useLink } from 'solito/link'

import {SheetDemo} from './sheet'

export function HomeScreen( {vibinchair}) {
  const linkProps = useLink({
    href: '/user/nate',
  })
  console.log('HIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII');
  console.log("Homescreen", vibinchair);

  //function to send message to websocket
  const send = (message) => {
    //maybe do a check to see if the socket is open & message would work
    vibinchair.send(JSON.stringify(message));
    //heyyyyyy cutie IM IN YOUR CODE NOW
  }


  
  return (
    <YStack f={1} jc="center" ai="center" p="$4" backgroundColor="#182030" space>
      <YStack space="$4" maw={600}>
        <H1 ta="center">Welcome to Vibin.</H1>
        <Paragraph ta="center">
          Connect to Vibin to get started.
        </Paragraph>
      </YStack>

      <XStack>
        <Button {...linkProps} theme="light">Link to user</Button>
        <Button onPress={() => send({type: "command", origin: "app", nId: Math.random(), target:"dsp", data:"nah" })}>Vibrate</Button>
      </XStack>
      <SheetDemo vibinchair={vibinchair}/>

    </YStack>
  )
}


