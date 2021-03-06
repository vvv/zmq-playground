{-# LANGUAGE OverloadedStrings #-}

-- | Hello World server.

module Main where

import Control.Concurrent (threadDelay)
import Control.Monad (forever)
import Control.Monad.IO.Class (liftIO)
import System.ZMQ4.Monadic
  ( Rep(..)
  , bind
  , receive
  , runZMQ
  , send
  , socket
  )

main :: IO ()
main = runZMQ $ do
    -- Socket to talk to clients
    responder <- socket Rep
    bind responder "tcp://*:5555"

    forever $ do
        _buffer <- receive responder
        liftIO $ do
            putStrLn "Received Hello"
            threadDelay 1000000 -- Do some 'work'
        send responder [] "World"
