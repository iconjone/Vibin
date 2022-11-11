import { Anchor, Button, H1, Paragraph, Separator, Sheet, XStack, YStack, Slider } from '@my/ui'
import { ChevronDown, ChevronUp, Divide } from '@tamagui/lucide-icons'
import React, { useState } from 'react'
import { useLink } from 'solito/link'

export function HomeScreen() {
  const linkProps = useLink({
    href: '/user/nate',
  })

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
      </XStack>

      <SheetDemo />
    </YStack>
  )
}

function SheetDemo() {
  const [open, setOpen] = useState(false)
  const [position, setPosition] = useState(0)
  return (
    <>
      <Button
        size="$6"
        icon={open ? ChevronDown : ChevronUp}
        circular
        onPress={() => setOpen((x) => !x)}
        theme="light"
        
      />
      <Sheet
        modal
        open={open}
        onOpenChange={setOpen}
        snapPoints={[80]}
        position={position}
        onPositionChange={setPosition}
        dismissOnSnapToBottom
      >
        <Sheet.Overlay />
        <Sheet.Frame ai="center" jc="center">
          <Sheet.Handle />
          <Paragraph ta="center">
         Set your levels for the chair below.
        </Paragraph>
        <Separator my={50} theme="light"/>

<YStack space={20}>

        <Slider size="$4" height={ open ? 300 :500} width={250} defaultValue={[50]} max={100} step={1} orientation="horizontal" theme="light">
          <Slider.Track>
            <Slider.TrackActive />
           </Slider.Track>
         <Slider.Thumb circular index={0} />
        </Slider>
        </YStack>
        
          <Button
            size="$6"
            circular
            icon={ChevronDown}
            onPress={() => {
              setOpen(false)
            }}
          />
        </Sheet.Frame>
      </Sheet>
    </>
  )
}
