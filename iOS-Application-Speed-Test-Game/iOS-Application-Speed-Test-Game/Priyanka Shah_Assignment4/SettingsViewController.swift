//
//  SettingsViewController.swift
//  Priyanka Shah_Assignment4
//
//  Created by Shah, Priyanka on 4/20/16.
//  Copyright © 2016 Shah, Priyanka. All rights reserved.
//

import UIKit

class SettingsViewController: UIViewController, UITextFieldDelegate {

        var currentUser:String = ""
    @IBOutlet weak var displayUser: UILabel!
    @IBOutlet weak var delayText: UITextField!
    @IBOutlet weak var currUserL: UILabel!
    @IBOutlet weak var defaultSText: UITextField!
    @IBOutlet weak var switchB: UIButton!
    override func viewDidLoad() {
        super.viewDidLoad()
        self.defaultSText.delegate = self
        self.delayText.delegate = self
        // Do any additional setup after loading the view.
        let defaults = NSUserDefaults.standardUserDefaults()

        if (defaults.stringForKey("currentUser") != nil)
        {
            currentUser = defaults.stringForKey("currentUser")!
            switchB.hidden = false
            displayUser.text = "Welcome \(currentUser)!"
            currUserL.text = currentUser
            defaultSText.text = defaults.stringForKey("defaultSpeed")
            delayText.text = defaults.stringForKey("delay")
        }
        else
        {
            currentUser = ""
            switchB.hidden = true
            
        }
    }
    
    override func viewWillAppear(animated: Bool) {
        let defaults = NSUserDefaults.standardUserDefaults()

        if (defaults.stringForKey("currentUser") != nil)
        {
            currentUser = defaults.stringForKey("currentUser")!
            switchB.hidden = false
            displayUser.text = "Welcome \(currentUser)!"
            currUserL.text = currentUser
            defaultSText.text = defaults.stringForKey("defaultSpeed")
            delayText.text = defaults.stringForKey("delay")
        }
        else
        {
            currentUser = ""
            switchB.hidden = true
            
        }
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
    /*func checkInput(input: String) -> Bool {
        
        let exp = "^\\d*$"
        let expCheck = NSPredicate(format: "SELF MATCHES %@", exp)
        return expCheck.evaluateWithObject(input)
    }*/
    
    func textField(textField: UITextField,
                   shouldChangeCharactersInRange range: NSRange,
                                                 replacementString string: String) -> Bool {
        //var res:Bool = false
        
            let newCharacters = NSCharacterSet(charactersInString: string)
            return NSCharacterSet.decimalDigitCharacterSet().isSupersetOfSet(newCharacters)
            
    }
    
    
    func showAlert(title first:String, message second:String, buttonTitle third:String)
    {
        let alert = UIAlertController(title: first, message: second, preferredStyle: .Alert)
        
        let close = UIAlertAction(title: third, style: .Cancel) { (action) in  }
        
        alert.addAction(close)
        self.presentViewController(alert, animated: true) { }
        
    }
    
    override func touchesBegan(touches: Set<UITouch>, withEvent event: UIEvent?) {
        let defaults = NSUserDefaults.standardUserDefaults()
        
        
        
                defaults.setObject(defaultSText.text!, forKey: "defaultSpeed")
        
        
        
        
                defaults.setObject(delayText.text!, forKey: "delay")
        
         defaultSText.resignFirstResponder()
        delayText.resignFirstResponder()
        self.view.endEditing(true)
        

    }
    func textFieldShouldReturn(textField: UITextField) -> Bool {
        let defaults = NSUserDefaults.standardUserDefaults()

        if(textField == defaultSText)
        {
                defaults.setObject(defaultSText.text!, forKey: "defaultSpeed")
            
        }
        
        if(textField == delayText)
        {
                defaults.setObject(delayText.text!, forKey: "delay")
            
        }
        
        textField.resignFirstResponder()
        
        return true
    }

   
    /*
    // MARK: - Navigation

    // In a storyboard-based application, you will often want to do a little preparation before navigation
    override func prepareForSegue(segue: UIStoryboardSegue, sender: AnyObject?) {
        // Get the new view controller using segue.destinationViewController.
        // Pass the selected object to the new view controller.
    }
    */
}
