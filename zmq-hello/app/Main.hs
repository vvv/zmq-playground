{-# LANGUAGE OverloadedStrings #-}

module Main where

import           Control.Concurrent (threadDelay)
import           Control.Monad (forever)
import           Control.Monad.IO.Class (liftIO)
import qualified Data.ByteString.Char8 as BS
import           System.ZMQ4.Monadic
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
        buffer <- receive responder
        liftIO $ do
            putStrLn "Received Hello"
            BS.putStrLn buffer
            threadDelay 1000000 -- Do some 'work'
        send responder [] "World"
