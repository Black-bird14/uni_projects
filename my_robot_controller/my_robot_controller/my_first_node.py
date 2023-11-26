#!/usr/bin/env python3
import rclpy
from rclpy.node import Node

class MyNode(Node):
    def __init__(self):
        super().__init__("first_node")
        self.counter_=0 #attribute of the class
        self.create_timer(1.0, self.timer_callback)
        
    def timer_callback(self):
        #in order for our program to actually do something
        self.get_logger().info("Hello "+str(self.counter_))
        self.counter_+=1

def main(args=None):
    #First we initialize ros communications, the args argument of main is passed to init
    #This is always the first line
    rclpy.init(args=args)
    #we initialize the node here, no need for parameters since we didn't add any in our constructor
    node= MyNode()
    rclpy.spin(node)
    #And this is always the last
    rclpy.shutdown()


if __name__=='__main__':
    main()
