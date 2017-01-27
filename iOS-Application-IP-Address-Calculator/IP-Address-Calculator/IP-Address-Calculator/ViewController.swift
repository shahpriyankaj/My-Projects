//
//  ViewController.swift
//  Assignment3_PriyankaShah
//
//  Created by Shah, Priyanka on 3/29/16.
//  Copyright © 2016 Shah, Priyanka. All rights reserved.
//

import UIKit

class ViewController: UIViewController, UITableViewDelegate, UITableViewDataSource, UITextFieldDelegate {

    @IBOutlet weak var ipText: UITextField!
    @IBOutlet weak var netText: UITextField!
    @IBOutlet weak var ipTable: UITableView!
    
    var s1:Subnetting!
    var ipList:[String] = []
    
    @IBAction func ipButton(sender: UIButton) {
        
        if validateFields() // Vlaidation of Subnet Bits and IP Address
        {
            let ipAddress = (ipText.text)!.componentsSeparatedByString(".")
            let ipAddress1 = Int(ipAddress[0])
            let ipAddress2 = Int(ipAddress[1])
            let ipAddress3 = Int(ipAddress[2])
            let ipAddress4 = Int(ipAddress[3])
                
            s1 = Subnetting(ipAddr1: ipAddress1!, ipAddr2: ipAddress2!, ipAddr3: ipAddress3!, ipAddr4: ipAddress4!, subnetBits: Int(netText.text!)!)
            let ipRange = s1.getIpRange()
            
            ipList = ipRange.ipList
            
            showAlert(title:"IP Addresses",message:"Subnet Mask: \(ipRange.subnetStr) \nFirst IP Address: \(ipRange.firstAddrStr) \nLast IP Address: \(ipRange.lastAddrStr)",buttonTitle:"OK")
        
            ipTable.hidden = false
            self.ipTable.reloadData()  // Reloading table View Data.
        }
        else
        {
            ipTable.hidden = true
        }
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        // Do any additional setup after loading the view, typically from a nib.
        self.ipText.delegate = self
        self.netText.delegate = self
        
        ipText.becomeFirstResponder()
        
        self.ipTable.delegate = self
        self.ipTable.dataSource = self
        
        ipTable.hidden = true
        
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
    
    override func touchesBegan(touches: Set<UITouch>, withEvent event: UIEvent?) {
        if !validateFields()
        {
            ipTable.hidden = true
        }
        else
        {
            ipText.resignFirstResponder()
            netText.resignFirstResponder()
            view.endEditing(true)
        }
        
    }
    
    func textFieldShouldReturn(textField: UITextField) -> Bool {
        
        if !validateFields(){
            ipTable.hidden = true
            return true
        }
        
        textField.resignFirstResponder()
        
        return true
    }
    
    func validateFields() -> Bool
    {
        //Checking for Valid Subnet Bits
        if !netText.text!.isEmpty && checkInput(netText.text!, whichInput: "SUBNET")
        {
            // Checking for Boundary Condition
            if Int(netText.text!)! < 16 || Int(netText.text!)! > 32
            {
            showAlert(title:"Error",message:"Subnet should be between 16 and 32.",buttonTitle:"Close")
            return false
            }
        }
        else
        {
            showAlert(title:"Error",message:"Invalid Subnet Mask.",buttonTitle:"Close")
            return false
        }
        
        let ipAddress = (ipText.text)!.componentsSeparatedByString(".")
        //Checking for Valid IP Address
        if ipAddress.count == 4 && checkInput(ipText.text!, whichInput: "IP")
        {
            
            let ipAddress1 = Int(ipAddress[0])
            let ipAddress2 = Int(ipAddress[1])
            let ipAddress3 = Int(ipAddress[2])
            let ipAddress4 = Int(ipAddress[3])
            
            // Checking for Boundary Condition
            if (ipAddress1 < 0 || ipAddress1 > 255){
                showAlert(title:"Error",message:"Invalid IP Address.",buttonTitle:"Close")
                return false
            }
            if (ipAddress2 < 0 || ipAddress2 > 255){
                showAlert(title:"Error",message:"Invalid IP Address.",buttonTitle:"Close")
                return false
            }
            if (ipAddress3 < 0 || ipAddress3 > 255){
                showAlert(title:"Error",message:"Invalid IP Address.",buttonTitle:"Close")
                return false
            }
            if (ipAddress4 < 0 || ipAddress4 > 255){
                showAlert(title:"Error",message:"Invalid IP Address.",buttonTitle:"Close")
                return false
            }
        }
        else
        {
            showAlert(title:"Error",message:"Invalid IP Address.",buttonTitle:"Close")
            return false
        }
        return true
        
    }
    
    func checkInput(input: String, whichInput: String) -> Bool {
        var exp:String

        if whichInput == "IP"
        {
            exp = "^([0-9]|[1-9][0-9]|[1][0-9][0-9]|[2][0-5][0-5])[.]([0-9]|[1-9][0-9]|[1][0-9][0-9]|[2][0-5][0-5])[.]([0-9]|[1-9][0-9]|[1][0-9][0-9]|[2][0-5][0-5])[.]([0-9]|[1-9][0-9]|[1][0-9][0-9]|[2][0-5][0-5])$"
        }
        else
        {
            exp = "^\\d*$"
        }
        let expCheck = NSPredicate(format: "SELF MATCHES %@", exp)
        return expCheck.evaluateWithObject(input)
    }
    
    
    func showAlert(title first:String, message second:String, buttonTitle third:String)
    {
        let alert = UIAlertController(title: first, message: second, preferredStyle: .Alert)
        
        let close = UIAlertAction(title: third, style: .Cancel) { (action) in  }
        
        alert.addAction(close)
        self.presentViewController(alert, animated: true) { }
     
    }
    
    func numberOfSectionsInTableView(tableView: UITableView) -> Int {
        return 1
    }
    
    func tableView(tableView: UITableView, titleForHeaderInSection section: Int) -> String? {
        return "IP Addresses"
    }
    
    func tableView(tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        return ipList.count
    }
    
    func tableView(tableView: UITableView, cellForRowAtIndexPath indexPath: NSIndexPath) -> UITableViewCell {
        let identifier = "ipAddress"
        let cell = tableView.dequeueReusableCellWithIdentifier(identifier, forIndexPath: indexPath) as UITableViewCell
        let tempIpAddr = ipList[indexPath.row]
        cell.textLabel!.text = tempIpAddr
        return cell
        
    }


}

