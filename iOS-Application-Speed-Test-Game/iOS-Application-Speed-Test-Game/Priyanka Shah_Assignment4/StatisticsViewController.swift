//
//  SecondViewController.swift
//  Priyanka Shah_Assignment4
//
//  Created by Shah, Priyanka on 4/20/16.
//  Copyright © 2016 Shah, Priyanka. All rights reserved.
//

import UIKit
import CoreData

class StatisticsViewController: UIViewController, UITableViewDataSource, UITableViewDelegate {

    var currentUser: String = ""
    var userDataTable:[AnyObject] = []

    
    @IBOutlet weak var welcomeL: UILabel!
    @IBOutlet weak var firstA: UILabel!
    @IBOutlet weak var secondA: UILabel!
    @IBOutlet weak var thirdA: UILabel!
    @IBOutlet weak var noTries: UILabel!
    @IBOutlet weak var userDetail: UITableView!
    override func viewDidLoad() {
        super.viewDidLoad()
        // Do any additional setup after loading the view, typically from a nib.
        self.userDetail.dataSource = self
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
    override func viewWillAppear(animated: Bool) {
        
        let defaults = NSUserDefaults.standardUserDefaults()
        if (defaults.stringForKey("currentUser") != nil)
        {
            currentUser = defaults.stringForKey("currentUser")!
            welcomeL.text = "Welcome \(currentUser)!"
        }
        
        displayLatestAttempts()
        displayUserSpeed()
       
        
    }
    
    func displayLatestAttempts()
    {
        var userData:[AnyObject] = []
        let appDelegate = UIApplication.sharedApplication().delegate as! AppDelegate
        let context =  appDelegate.managedObjectContext
        
        //print("latest attempt")
        var fetchResult = NSFetchRequest(entityName: "UserDetails")
        var predicate = NSPredicate(format: "username = %@ ", currentUser)
        fetchResult.predicate = predicate
        fetchResult.resultType = .DictionaryResultType
        
        fetchResult.propertiesToFetch = ["username", "speed"]
        let sort = NSSortDescriptor(key: "dateTime", ascending: false)
        
       
        fetchResult.fetchLimit = 3
        fetchResult.sortDescriptors = [sort]
        //print("latest attempt1")
        do {
            userData = try context.executeFetchRequest(fetchResult)
            //print("latest attempt2")
            switch(userData.count){
            case 1:
                //print(userData[0].valueForKey("username")!.description)

                    self.firstA.text = userData[0].valueForKey("speed")!.description
                    //print(userData[0].valueForKey("speed")!.description)
                break
                
            case 2:
                //print(userData[0].valueForKey("username")!.description)

                self.firstA.text = userData[0].valueForKey("speed")!.description
                //print(userData[0].valueForKey("speed")!.description)
                self.secondA.text = userData[1].valueForKey("speed")!.description
               // print(userData[1].valueForKey("speed")!.description)
                    break
            
            case 3:
                //print(userData[0].valueForKey("username")!.description)
                self.firstA.text = userData[0].valueForKey("speed")!.description
               // print(userData[0].valueForKey("speed")!.description)
                self.secondA.text = userData[1].valueForKey("speed")!.description
               // print(userData[1].valueForKey("speed")!.description)
                self.thirdA.text = userData[2].valueForKey("speed")!.description
               // print(userData[2].valueForKey("speed")!.description)
                    break
            
            default:
                break
              
                }
            
            }
            
        catch {
            let resultError = error as NSError
            print(resultError)
        }
        
        // No of Tries
        
        fetchResult = NSFetchRequest(entityName: "UserDetails")
        predicate = NSPredicate(format: "username = %@ ", currentUser)
        fetchResult.predicate = predicate
        fetchResult.resultType = .DictionaryResultType
        let expression = NSExpression(format: "count:(speed)")
        let expDesc = NSExpressionDescription()
        expDesc.expression = expression
        expDesc.name = "countTries"
        expDesc.expressionResultType = .Integer32AttributeType
        
        fetchResult.propertiesToFetch = ["username", expDesc]
        fetchResult.propertiesToGroupBy = ["username"]
        
        
        
        do {
            userData = try context.executeFetchRequest(fetchResult)
            if !userData.isEmpty{
                noTries.text = userData[0].valueForKey("countTries")!.description
                noTries.text = String(Int(noTries.text!)! - 1)
            }
            
        } catch {
            let resultError = error as NSError
            print(resultError)
        }
        

        
    }
    
    func displayUserSpeed()
    {
        let appDelegate = UIApplication.sharedApplication().delegate as! AppDelegate
        let context =  appDelegate.managedObjectContext
        
        let fetchResult = NSFetchRequest(entityName: "UserDetails")
        fetchResult.resultType = .DictionaryResultType
        let expression = NSExpression(format: "max:(speed)")
        let expDesc = NSExpressionDescription()
        expDesc.expression = expression
        expDesc.name = "TopSpeed"
        expDesc.expressionResultType = .Integer32AttributeType
        
        fetchResult.propertiesToFetch = ["username", expDesc]
        fetchResult.propertiesToGroupBy = ["username"]
        let sort = NSSortDescriptor(key: "speed", ascending: false)
        
        fetchResult.sortDescriptors = [sort]
        
        do {
            userDataTable = try context.executeFetchRequest(fetchResult)
            
        } catch {
            let resultError = error as NSError
            print(resultError)
        }
        
        self.userDetail.reloadData() // Reload table data
    }
    
    func numberOfSectionsInTableView(tableView: UITableView) -> Int {
        return 1
    }
    
    func tableView(tableView: UITableView, titleForHeaderInSection section: Int) -> String? {
        return "TopSpeed of the Users"
    }
    
    func tableView(tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        return userDataTable.count
    }
    
    func tableView(tableView: UITableView, cellForRowAtIndexPath indexPath: NSIndexPath) -> UITableViewCell {
        let contactIdentifier = "userList"
        let cell = tableView.dequeueReusableCellWithIdentifier(contactIdentifier, forIndexPath: indexPath)
        let object = userDataTable[indexPath.row]
       // print(indexPath.row)
       
       
        cell.textLabel!.text = "UserName: " + object.valueForKey("username")!.description
        cell.detailTextLabel?.text = "TopSpeed: " + object.valueForKey("TopSpeed")!.description
        return cell
    }



}

