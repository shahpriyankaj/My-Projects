//
//  UserTableViewController.swift
//  Priyanka Shah_Assignment4
//
//  Created by Shah, Priyanka on 4/20/16.
//  Copyright © 2016 Shah, Priyanka. All rights reserved.
//

import UIKit
import CoreData

class UserTableViewController: UIViewController, UITableViewDataSource, UITableViewDelegate {

    var selectedUser: String = ""
    var userDataTable:[AnyObject] = []
    @IBOutlet weak var userTable: UITableView!
    
    override func viewDidLoad() {
        super.viewDidLoad()
        self.userTable.dataSource = self
        self.userTable.delegate = self
        displayUserNames()
        self.userTable.reloadData()
        // Do any additional setup after loading the view.
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
    func displayUserNames()
    {
        let appDelegate = UIApplication.sharedApplication().delegate as! AppDelegate
        let context =  appDelegate.managedObjectContext
        
        let fetchResult = NSFetchRequest(entityName: "UserDetails")
        fetchResult.resultType = .DictionaryResultType
        
        fetchResult.propertiesToFetch = ["username"]
        fetchResult.propertiesToGroupBy = ["username"]
        
        do {
            userDataTable = try context.executeFetchRequest(fetchResult)
            
        } catch {
            let resultError = error as NSError
            print(resultError)
        }
        
        self.userTable.reloadData() // Reload table data
    }
    
    func numberOfSectionsInTableView(tableView: UITableView) -> Int {
        return 1
    }
    
    func tableView(tableView: UITableView, titleForHeaderInSection section: Int) -> String? {
        return "List of Users"
    }
    
    func tableView(tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        return userDataTable.count
    }
    
    func tableView(tableView: UITableView, cellForRowAtIndexPath indexPath: NSIndexPath) -> UITableViewCell {
        let contactIdentifier = "userNameList"
        let cell = tableView.dequeueReusableCellWithIdentifier(contactIdentifier, forIndexPath: indexPath)
        let object = userDataTable[indexPath.row]
        //print(indexPath.row)
        cell.textLabel!.text = object.valueForKey("username")!.description
        return cell
    }
    
    func tableView(tableView: UITableView, didSelectRowAtIndexPath indexPath: NSIndexPath) {
        print("inside")
        selectedUser = (tableView.cellForRowAtIndexPath(indexPath))!.textLabel!.text!
        print(indexPath.row)
        print(selectedUser)
        self.performSegueWithIdentifier("showCurrUser", sender: self)

    }
    /*
    func tableView(tableView: UITableView, didDeselectRowAtIndexPath indexPath: NSIndexPath)
    {
        print("inside")
        selectedUser = (tableView.cellForRowAtIndexPath(indexPath))!.textLabel!.text!
        print(indexPath.row)
        print(selectedUser)
        self.performSegueWithIdentifier("showCurrUser", sender: self)
        

    }*/
    
    override func prepareForSegue(segue: UIStoryboardSegue?, sender: AnyObject?) {
        let defaults = NSUserDefaults.standardUserDefaults()
        
        defaults.setObject(selectedUser, forKey: "currentUser")
    }
    
    
    /*func tableView(tableView: UITableView, didSelectRowAtIndexPath indexPath: NSIndexPath) {
        
        let defaults = NSUserDefaults.standardUserDefaults()
        //let indexPath = tableView.indexPathForSelectedRow!
        
      //  let currentCell = tableView.cellForRowAtIndexPath(indexPath)! as UITableViewCell
       // let user = currentCell.textLabel!.text
        
        let user = tableView.cellForRowAtIndexPath(indexPath)?.textLabel?.text
                print(user)
        
        defaults.setObject(user, forKey: "currentUser")
        /*if (defaults.stringForKey("currentUser") != nil)
        {
            currentUser = defaults.stringForKey("currentUser")!
            print("\(currentUser)")
        }*/
        self.performSegueWithIdentifier("showCurrUser", sender: self)

    }*/
    
    @IBAction func reloadUsers(sender: UIStoryboardSegue)
    {
        displayUserNames()
        self.userTable.reloadData()
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
