//
//  FirstViewController.swift
//  Priyanka Shah_Assignment4
//
//  Created by Shah, Priyanka on 4/20/16.
//  Copyright © 2016 Shah, Priyanka. All rights reserved.
//

import UIKit
import CoreData

class SpeedTestViewController: UIViewController,  NSFetchedResultsControllerDelegate, UITextFieldDelegate{

    var managedObjectContext: NSManagedObjectContext? = nil
    //let defaults = NSUserDefaults.standardUserDefaults()

    var currentUser: String = ""
    var timer = NSTimer()
    
    var counter = 0
    
    var defaultSpeed:String = ""
    var delay:String = ""
  
    @IBOutlet weak var displayUser: UILabel!
   
    @IBOutlet weak var wpmLabel: UILabel!
    @IBOutlet weak var userLabel: UILabel!
    @IBOutlet weak var userText: UITextField!
    @IBOutlet weak var wpmText: UITextField!
    @IBOutlet weak var displayWord: UILabel!
    
    @IBOutlet weak var stopB: UIButton!
    @IBOutlet weak var startB: UIButton!
   
    @IBAction func start(sender: UIButton) {
        timer.invalidate()
        if(!(wpmText.text?.isEmpty)!) && Int(wpmText.text!) > 0{
            storeUserDetails()
            let interval = calculateInterval()
            print(interval)
            if !delay.isEmpty
            {
                sleep(UInt32(delay)!)
            }
        // Starting Timer to display word
            timer = NSTimer.scheduledTimerWithTimeInterval(interval, target: self, selector: #selector(SpeedTestViewController.displayWords), userInfo: nil, repeats: true)
        }
        else
        {
            showAlert(title:"Invalid WPM Speed",message:"Kindly Enter Valid WPM Speed",buttonTitle:"CLOSE")
     
        }
    }
    
    // Validating WPM Speed field
    /*func checkInput(input: String) -> Bool {
        
        let exp = "^\\d*$"
        let expCheck = NSPredicate(format: "SELF MATCHES %@", exp)
        return expCheck.evaluateWithObject(input)
    }*/

    
    func showAlert(title first:String, message second:String, buttonTitle third:String)
    {
        let alert = UIAlertController(title: first, message: second, preferredStyle: .Alert)
        
        let close = UIAlertAction(title: third, style: .Cancel) { (action) in  }
        
        alert.addAction(close)
        self.presentViewController(alert, animated: true) { }
        
    }
    
    func displayWords(){
        
        if let path = NSBundle.mainBundle().pathForResource("data", ofType: "txt")
        {
            //print(path)
            
            if let text = try? String(contentsOfFile: path, encoding: NSUTF8StringEncoding)
            {
                //print(text)
                let para = text.componentsSeparatedByString("~")
                let paraData = para[Int(arc4random_uniform(UInt32(para.count)))]
                let fileData = paraData.componentsSeparatedByString(" ")
                let word = fileData[Int(arc4random_uniform(UInt32(fileData.count)))]
                
                let loc = Int(word.characters.count / 2)
                
                
                let wordColor = NSMutableAttributedString(string: word)
                
                // change the color atribute
                
                wordColor.addAttribute(NSForegroundColorAttributeName, value: UIColor.greenColor(), range: NSRange(location: loc, length: 1))
                displayWord.attributedText = wordColor
            }
        }
        // code for fetching word from file
        
        
    }
    
    func calculateInterval() -> Double
    {
        let wpm = Double(wpmText.text!)
        return (60/wpm!)
    }
    
    func storeUserDetails()
    {
        let appDelegate = UIApplication.sharedApplication().delegate as! AppDelegate
        let context =  appDelegate.managedObjectContext
        let newManagedObject = NSEntityDescription.insertNewObjectForEntityForName("UserDetails", inManagedObjectContext: context)
    
        var speed:Int = 0
        if !(wpmText.text?.isEmpty)!{
            speed = Int(wpmText.text!)!
        }
        newManagedObject.setValue(currentUser, forKey: "username")
        newManagedObject.setValue(speed, forKey: "speed")
        newManagedObject.setValue(NSDate(), forKey: "dateTime")
    
    // Save the context.
        do
        {
            try context.save()
        }
        catch
        {
            abort()
        }

    }
    
    @IBAction func stop(sender: UIButton) {
        // Storing USer detaisl in the Core Data
        timer.invalidate()
        
        displayWord.text = ""
        print(displayWord.text!)
        /*if(!(wpmText.text?.isEmpty)!) && Int(wpmText.text!) > 0{
            storeUserDetails()
        }
        else
        {
            showAlert(title:"Invalid WPM Speed",message:"Kindly Enter Valid WPM Speed",buttonTitle:"CLOSE")
            
        }*/
    }
    
    override func viewWillAppear(animated: Bool) {
        let defaults = NSUserDefaults.standardUserDefaults()
        if (defaults.stringForKey("currentUser") != nil)
        {
            currentUser = defaults.stringForKey("currentUser")!
            print("\(currentUser)")
            userLabel.hidden = true
            userText.hidden = true
            wpmLabel.hidden = false
            wpmText.hidden = false
            displayWord.hidden = false
            startB.hidden = false
            stopB.hidden = false
            userText.text = currentUser
            displayUser.text = "Welcome \(currentUser)!"
            if (defaults.stringForKey("defaultSpeed") != nil)
            {
                defaultSpeed = defaults.stringForKey("defaultSpeed")!
            }
            if (defaults.stringForKey("delay") != nil)
            {
                delay = defaults.stringForKey("delay")!
            }
            //delay = defaults.stringForKey("delay")!
            wpmText.text = defaultSpeed
        }
        else
        {
            currentUser = ""
            print("\(currentUser)")
            wpmLabel.hidden = true
            wpmText.hidden = true
            displayWord.hidden = true
            startB.hidden = true
            stopB.hidden = true
            userLabel.hidden = false
            userText.hidden = false
            
        }

        
    }
    
    
    override func viewDidLoad() {
        super.viewDidLoad()
        // Do any additional setup after loading the view, typically from a nib.
        
        self.userText.delegate = self
        self.wpmText.delegate = self
        
        let defaults = NSUserDefaults.standardUserDefaults()
        if (defaults.stringForKey("currentUser") != nil)
        {
            currentUser = defaults.stringForKey("currentUser")!
           
            userLabel.hidden = true
            userText.hidden = true
            wpmLabel.hidden = false
            wpmText.hidden = false
            displayWord.hidden = false
            startB.hidden = false
            stopB.hidden = false
            
            userText.text = currentUser
            displayUser.text = "Welcome \(currentUser)!"
            if (defaults.stringForKey("defaultSpeed") != nil)
            {
                defaultSpeed = defaults.stringForKey("defaultSpeed")!
            }
            if (defaults.stringForKey("delay") != nil)
            {
                delay = defaults.stringForKey("delay")!
            }
            //delay = defaults.stringForKey("delay")!
            wpmText.text = defaultSpeed
        }
        else
        {
            currentUser = ""
            wpmLabel.hidden = true
            wpmText.hidden = true
            displayWord.hidden = true
            startB.hidden = true
            stopB.hidden = true
            userLabel.hidden = false
            userText.hidden = false
            
        }
        
    }
    func textField(textField: UITextField,
                   shouldChangeCharactersInRange range: NSRange,
                                                 replacementString string: String) -> Bool {
        //var res:Bool = false
        if(textField == wpmText){
        
        let newCharacters = NSCharacterSet(charactersInString: string)
        return NSCharacterSet.decimalDigitCharacterSet().isSupersetOfSet(newCharacters)
        }
        return true
        
    }
    
    func textFieldShouldReturn(textField: UITextField) -> Bool {
        
        if(textField == userText)
        {
            if !(userText.text?.isEmpty)!
            {
                userLabel.hidden = true
                userText.hidden = true
                wpmLabel.hidden = false
                wpmText.hidden = false
                displayWord.hidden = false
                startB.hidden = false
                stopB.hidden = false
                displayUser.text = "Welcome \(userText.text!)!"
            
                let defaults = NSUserDefaults.standardUserDefaults()
            
                defaults.setObject(userText.text!, forKey: "currentUser")
                if (defaults.stringForKey("currentUser") != nil)
                {
                    currentUser = defaults.stringForKey("currentUser")!
                    print("\(currentUser)")
                }
                if !getUserName(){
                    storeUserDetails()
                }
            }
            else{
                showAlert(title:"Invalid Username",message:"Kindly Enter Valid Username",buttonTitle:"CLOSE")
            }
            
            
        }
        //print("nothing")
        textField.resignFirstResponder()
        
        return true
    }
    
    override func touchesBegan(touches: Set<UITouch>, withEvent event: UIEvent?) {
     
        if !(userText.text?.isEmpty)!
        {
            userLabel.hidden = true
            userText.hidden = true
            wpmLabel.hidden = false
            wpmText.hidden = false
            displayWord.hidden = false
            startB.hidden = false
            stopB.hidden = false
            displayUser.text = "Welcome \(userText.text!)!"
            
            let defaults = NSUserDefaults.standardUserDefaults()
            
            defaults.setObject(userText.text!, forKey: "currentUser")
            if (defaults.stringForKey("currentUser") != nil)
            {
                currentUser = defaults.stringForKey("currentUser")!
                print("\(currentUser)")
            }
            if !getUserName(){
                storeUserDetails()
            }
            
            
            
        }
        else
        {
            showAlert(title:"Invalid Username",message:"Kindly Enter Valid Username",buttonTitle:"CLOSE")
        }
        userText.resignFirstResponder()
        wpmText.resignFirstResponder()
        self.view.endEditing(true)
        //print("nothing")
        
    }
    func getUserName() -> Bool
    {
        var userDataTable:[AnyObject] = []
        let appDelegate = UIApplication.sharedApplication().delegate as! AppDelegate
        let context =  appDelegate.managedObjectContext
        
        let fetchResult = NSFetchRequest(entityName: "UserDetails")
        
        let predicate = NSPredicate(format: "username = %@ ", userText.text!)
        fetchResult.predicate = predicate
        fetchResult.resultType = .DictionaryResultType
        fetchResult.propertiesToFetch = ["username"]
        
        do {
            userDataTable = try context.executeFetchRequest(fetchResult)
            
            
        }
        catch {
            let resultError = error as NSError
            print(resultError)
        }
        if !userDataTable.isEmpty{
            return true
        }
        return false
        
    }
    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }


}

