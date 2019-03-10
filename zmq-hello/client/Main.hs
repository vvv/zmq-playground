{-# LANGUAGE OverloadedStrings #-}

-- | Hello World client.

module Main where

import Control.Monad.IO.Class (liftIO)
import Data.Foldable (for_)
import System.ZMQ4.Monadic
  ( Req(..)
  , connect
  , receive
  , runZMQ
  , send
  , socket
  )

main :: IO ()
main = runZMQ $ do
    liftIO $ putStrLn "Connecting to hello world server…"

    requester <- socket Req
    connect requester "tcp://localhost:5555"

    for_ [(1 :: Int)..10] $ \i -> do
        liftIO . putStrLn $ "Sending Hello " ++ show i ++ "…"
        send requester [] "Hello"
        _ <- receive requester
        liftIO . putStrLn $ "Received World " ++ show i
