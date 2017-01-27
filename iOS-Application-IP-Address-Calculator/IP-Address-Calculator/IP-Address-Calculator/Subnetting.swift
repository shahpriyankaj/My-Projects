//
//  Subnetting.swift
//  Assignment3_PriyankaShah
//
//  Created by Shah, Priyanka on 3/29/16.
//  Copyright © 2016 Shah, Priyanka. All rights reserved.
//

import Foundation

class Subnetting
{
    var ipAddr1:Int
    var ipAddr2:Int
    var ipAddr3:Int
    var ipAddr4:Int
    var subnetBits:Int
    
    init(ipAddr1 first:Int, ipAddr2 second:Int, ipAddr3 third:Int, ipAddr4 forth:Int, subnetBits fifth:Int)
    {
        ipAddr1 = first
        ipAddr2 = second
        ipAddr3 = third
        ipAddr4 = forth
        subnetBits = fifth
    }
    
    
    func getIpRange() -> (firstAddrStr:String, lastAddrStr:String, subnetStr:String, ipList: [String])
    {
        // This function gives Ip Address Ranges
        
        // Calculates Subnet Mask
        let subnetMask = getSubnetMask()
        
        let subnet1 = UInt8(subnetMask.subnet1)
        let subnet2 = UInt8(subnetMask.subnet2)
        let subnet3 = UInt8(subnetMask.subnet3)
        let subnet4 = UInt8(subnetMask.subnet4)
        
        // Calculates First IP Address
        let firstAddr = findFirstAddress(subnet1, second: subnet2, third: subnet3, forth: subnet4)
        
        // Calculates Last IP Address
        var lastAddr = findLastAddress(subnet1, second: subnet2, third: subnet3, forth: subnet4, fifth: UInt8(ipAddr1), sixth: UInt8(ipAddr2), seventh: UInt8(ipAddr3), eighth: UInt8(ipAddr4) )
        
        
        // If subnet Bits are equal to 32, then First and last address will be same.
        if subnetBits == 32
        {
            lastAddr.lastAddr1 = firstAddr.firstAddr1
            lastAddr.lastAddr2 = firstAddr.firstAddr2
            lastAddr.lastAddr3 = firstAddr.firstAddr3
            lastAddr.lastAddr4 = firstAddr.firstAddr4
        }
        
        let firstAddrStr = String((firstAddr.firstAddr1)) + "." + String(firstAddr.firstAddr2) + "." + String(firstAddr.firstAddr3) + "." + String(firstAddr.firstAddr4)
        
        
        let lastAddrStr = String(lastAddr.lastAddr1) + "." + String(lastAddr.lastAddr2) + "." + String(lastAddr.lastAddr3) + "." + String(lastAddr.lastAddr4)
        
        let subnetStr = "\(subnet1).\(subnet2).\(subnet3).\(subnet4)"
        
        print("first Address: \(firstAddrStr)")
        print("last Address: \(lastAddrStr)")
        print("subnet Address: \(subnetStr)")
        
        // Get an array of Range of IP Addresses
        let ipList = getIpRange(firstAddr.firstAddr1, second: firstAddr.firstAddr2, third: firstAddr.firstAddr3, forth: firstAddr.firstAddr4, fifth: lastAddr.lastAddr1, sixth: lastAddr.lastAddr2, seventh: lastAddr.lastAddr3, eighth: lastAddr.lastAddr4 )
        
        
        return (firstAddrStr, lastAddrStr, subnetStr, ipList)
        
    }
    
    func getIpRange(first:UInt8, second:UInt8, third:UInt8, forth:UInt8, fifth: UInt8, sixth: UInt8, seventh:UInt8, eighth:UInt8) -> [String]
    {
        // This function saves range of IP Addresses in an array and returns array to calling function.
        
        var ipList:[String] = []
        
        let ip1:Int = Int(first)
        let ip2:Int = Int(second)
        let ip3:Int = Int(third)
        let ip4:Int = Int(forth)
        let lIp1:Int = Int(fifth)
        let lIp2:Int = Int(sixth)
        let lIp3:Int = Int(seventh)
        let lIp4:Int = Int(eighth)
        
        var a = ip1
        while(a <= lIp1)
        {
            var b = ip2
            while(b <= lIp2)
            {
                var c = ip3
                while(c <= lIp3)
                {
                    var d = ip4
                    while(d <= lIp4)
                    {
                        let tempAdd = "\(a).\(b).\(c).\(d)"
                        ipList.append(tempAdd)
                        d++
                    }
                    c++
                }
                b++
            }
            a++
        }
    
        return ipList

    }
    
    func getSubnetMask() -> (subnet1:UInt, subnet2:UInt, subnet3:UInt, subnet4:UInt)
    {
        // This function calculates subnet Mask based on number of bits entered by User.
        let maxBits = 32
        
        let firstSubnet = String(count: subnetBits, repeatedValue: Character("1"))
        let remSubnet = String(count: (maxBits - subnetBits), repeatedValue: Character("0"))
        
        print(firstSubnet)
        print(remSubnet)
        let subnet  = firstSubnet + remSubnet
        
        let subnet1:String = subnet.substringWithRange(Range(start: subnet.startIndex,end: subnet.startIndex.advancedBy(8)))
        let subnet2:String = subnet.substringWithRange(Range(start: subnet.startIndex.advancedBy(8),end: subnet.startIndex.advancedBy(16)))
        let subnet3:String = subnet.substringWithRange(Range(start: subnet.startIndex.advancedBy(16),end: subnet.startIndex.advancedBy(24)))
        let subnet4:String = subnet.substringWithRange(Range(start: subnet.startIndex.advancedBy(24),end: subnet.endIndex))
        
        
        return ((strtoul(subnet1, nil, 2)), (strtoul(subnet2, nil, 2)), (strtoul(subnet3, nil, 2)), (strtoul(subnet4, nil, 2)))
        
    }
    
    func findFirstAddress(first:UInt8, second:UInt8, third:UInt8, forth:UInt8) -> (firstAddr1: UInt8, firstAddr2: UInt8, firstAddr3:UInt8, firstAddr4:UInt8)
    {
        // This function calculates first IP Address in IP Range.
        let firstAddr1 = UInt8(ipAddr1) & first
        let firstAddr2 = UInt8(ipAddr2) & second
        let firstAddr3 = UInt8(ipAddr3) & third
        var firstAddr4 = (UInt8(ipAddr4) & forth)
        
        // As 0 not allowed, changing vlaue to 1.
        if firstAddr4 == 0 && subnetBits != 32
        {
            firstAddr4 = 1
        }
        if firstAddr4 == 255 && subnetBits != 32
        {
            firstAddr4 = 254
        }
        
        return(firstAddr1, firstAddr2, firstAddr3, firstAddr4)
    }
    
    func findLastAddress(first:UInt8, second:UInt8, third:UInt8, forth:UInt8, fifth:UInt8, sixth:UInt8, seventh:UInt8, eighth:UInt8) -> (lastAddr1: UInt8, lastAddr2: UInt8, lastAddr3:UInt8, lastAddr4:UInt8)
    {
         // This function calculates last IP Address in IP Range.
        let lastAddr1 = fifth | ~first
        let lastAddr2 = sixth | ~second
        let lastAddr3 = seventh | ~third
        var lastAddr4 = eighth | ~forth
        
        if lastAddr4 == 0 && subnetBits != 32
        {
            lastAddr4 = 1
        }
         // As 255 not allowed, changing vlaue to 254.
        if lastAddr4 == 255 && subnetBits != 32
        {
            lastAddr4 = 254
        }
        
        return (lastAddr1, lastAddr2, lastAddr3, lastAddr4)
    }
    
    func getIpAddress() -> String
    {
        // This function converts binary to decimal and returns entire address.
        var ip1 = String(ipAddr1, radix:2)
        ip1 = formatIp(ip1)
        
        var ip2 = String(ipAddr2, radix:2)
        ip2 = formatIp(ip2)
        
        var ip3 = String(ipAddr3, radix:2)
        ip3 = formatIp(ip3)
        
        var ip4 = String(ipAddr4, radix:2)
        ip4 = formatIp(ip4)
        
        return ip1 + ip2 + ip3 + ip4
    }
    
    func formatIp(first:String) -> String
    {
        // This function will append zero to string if input is less than 8 bits.
        if first.characters.count < 8
        {
            let tempBits = String(count: (8 - first.characters.count), repeatedValue: Character("0"))
            return tempBits + first
        }
        return first
    }
    
    
}
