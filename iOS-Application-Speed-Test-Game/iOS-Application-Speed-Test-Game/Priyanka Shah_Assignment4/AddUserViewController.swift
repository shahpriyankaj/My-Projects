//
//  AddUserViewController.swift
//  Priyanka Shah_Assignment4
//
//  Created by Shah, Priyanka on 4/26/16.
//  Copyright © 2016 Shah, Priyanka. All rights reserved.
//

import UIKit
import CoreData

class AddUserViewController: UIViewController {

    @IBOutlet weak var userT: UITextField!
    override func viewDidLoad() {
        super.viewDidLoad()

        // Do any additional setup after loading the view.
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
    
    @IBAction func addUser(sender: UIButton) {
        
        if !getUserName()
        {
            let appDelegate = UIApplication.sharedApplication().delegate as! AppDelegate
            let context =  appDelegate.managedObjectContext
            let newManagedObject = NSEntityDescription.insertNewObjectForEntityForName("UserDetails", inManagedObjectContext: context)
            
        
            newManagedObject.setValue(userT.text, forKey: "username")
            newManagedObject.setValue(0, forKey: "speed")
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
        else{
             showAlert(title:"User already Exists",message:"User already exists.\nKindly Enter New User Name",buttonTitle:"CLOSE")
            
        }
        
        
    }
    func showAlert(title first:String, message second:String, buttonTitle third:String)
    {
        let alert = UIAlertController(title: first, message: second, preferredStyle: .Alert)
        
        let close = UIAlertAction(title: third, style: .Cancel) { (action) in  }
        
        alert.addAction(close)
        self.presentViewController(alert, animated: true) { }
        
    }
    
    func getUserName() -> Bool
    {
        var userDataTable:[AnyObject] = []
        let appDelegate = UIApplication.sharedApplication().delegate as! AppDelegate
        let context =  appDelegate.managedObjectContext
    
        let fetchResult = NSFetchRequest(entityName: "UserDetails")
        
        let predicate = NSPredicate(format: "username = %@ ", userT.text!)
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

    /*
    // MARK: - Navigation

    // In a storyboard-based application, you will often want to do a little preparation before navigation
    override func prepareForSegue(segue: UIStoryboardSegue, sender: AnyObject?) {
        // Get the new view controller using segue.destinationViewController.
        // Pass the selected object to the new view controller.
    }
    */

}
