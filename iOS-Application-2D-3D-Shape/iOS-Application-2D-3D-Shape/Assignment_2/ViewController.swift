//
//  ViewController.swift
//  Assignment_2
//
//  Created by Shah, Priyanka on 2/19/16.
//  Copyright © 2016 Shah, Priyanka. All rights reserved.
//

import UIKit

class ViewController: UIViewController, UIPickerViewDelegate, UIPickerViewDataSource, UITextFieldDelegate{

    @IBOutlet weak var selectShape: UISegmentedControl!
    @IBOutlet weak var selectMenu: UIPickerView!
    
    @IBOutlet weak var enterRad: UILabel!
    @IBOutlet weak var selectMenu1: UIPickerView!
    @IBOutlet var input: UIView!
    @IBOutlet weak var stepVal: UILabel!

    @IBOutlet weak var slide: UISlider!
    @IBOutlet weak var displayResult: UITextView!
    @IBOutlet weak var enterL1: UILabel!
    @IBOutlet weak var enterL2: UILabel!
    @IBOutlet weak var enterL3: UILabel!
    @IBOutlet weak var enterL4: UILabel!
    @IBOutlet weak var enterT1: UITextField!
    @IBOutlet weak var enterT2: UITextField!
    @IBOutlet weak var enterT3: UITextField!
    @IBOutlet weak var enterT4: UITextField!
    @IBOutlet weak var slideVal: UILabel!
    @IBOutlet weak var enterSlide: UISlider!
    @IBOutlet weak var enterHeight: UILabel!
    @IBOutlet weak var enterStep: UIStepper!
    @IBOutlet weak var shapeImage: UIImageView!
    
    var menuShapes = Array<String>()
    var selectedRow:String = ""
    var is2DShape:Bool = true
    var area:Double = 0.0
    var perimeter:Double = 0.0
    var volume:Double = 0.0
    let pi = 3.14
    
    override func viewDidLoad() {
        super.viewDidLoad()
        // Do any additional setup after loading the view, typically from a nib.
        selectMenu1.delegate = self
        selectMenu1.dataSource = self
        enterT1.delegate = self
        enterT2.delegate = self
        enterT3.delegate = self
        enterT4.delegate = self
        menuShapes =  ["Circle","Square","Rectangle","Trapezoid","Triangle"]
        hideAllFields()
        
    }
    func hideAllFields()
    {
        // This function will hide all the fields.
        enterRad.hidden = true
        enterStep.hidden = true
        stepVal.hidden = true
        enterHeight.hidden = true
        enterSlide.hidden = true
        slideVal.hidden = true
        enterL1.hidden = true
        enterL2.hidden = true
        enterL3.hidden = true
        enterL4.hidden = true
        enterT1.hidden = true
        enterT2.hidden = true
        enterT3.hidden = true
        enterT4.hidden = true
        shapeImage.hidden = true
    }
    @IBAction func clearButton(sender: AnyObject) {
        
        resetAllFields()
        
    }

    @IBAction func slideChange(sender: UISlider) {
        let shapeHeight = sender.value
        slideVal.text = String(format:"%.1f", shapeHeight)
        if area != 0.0 && perimeter != 0.0
        {
            // if Area and perimeter are already cacluated, then it will calculate when slider changes.
            calculateAllShapes()
        }
    }
    @IBAction func stepChange(sender: UIStepper) {
        stepVal.text = "\(Int(enterStep.value))"
        if area != 0.0 && perimeter != 0.0
        {
                // if Area and perimeter are already cacluated, then it will calculate when stepper value changes.
            calculateAllShapes()
        }
    }
    @IBAction func menuFunc(sender: UISegmentedControl) {
        if selectShape.selectedSegmentIndex == 0{
            is2DShape = true
            menuShapes =  ["Circle","Square","Rectangle","Trapezoid","Triangle"]
        }
        else{
            is2DShape = false
            menuShapes =  ["Cube","Pyramid","Prism","Cone","Cylinder","Sphere"]
            
        }
        selectMenu1.reloadAllComponents()
    }
    @IBAction func calculate(sender: UIButton) {
        area = 0.0
        perimeter = 0.0
        
        //Check whether shape has been selected or not.
        if selectedRow.isEmpty == true
        {
            displayResult.text = "There is no shape selected. Kindly Select Valid Shape."
        }
        else
        {
            calculateAllShapes()
        }
        
    }
    
    func checkText(inputText: String) -> Bool {
        // This function will check whether input is regular expression or not.
        let regExp = "\\d+\\.?\\d*"
        let checkExp = NSPredicate(format: "SELF MATCHES %@", regExp)
        return checkExp.evaluateWithObject(inputText)
    }
    
    func calculateAllShapes()
    {
        // Calculates Area and perimeter of 2D shapes and Surface Area and volume of 3D Shapes.
        var validText = true
        var imgPath:String = ""
        shapeImage.hidden = false
        switch(selectedRow){
            case "Circle":
                if stepVal.text?.isEmpty == false &&
                    Double(stepVal.text!)! > 0
                {
                    area = pi * Double(stepVal.text!)! * Double(stepVal.text!)!
                    perimeter = 2 * pi * Double(stepVal.text!)!
                    imgPath = "circle.png"
                }
                else
                {
                    validText = false
                }
                break
            case "Square":
                if stepVal.text?.isEmpty == false &&
                    Double(stepVal.text!)! > 0
                {
                    area = Double(stepVal.text!)! * Double(stepVal.text!)!
                    perimeter = 4 * Double(stepVal.text!)!
                    imgPath = "square.png"
                }
                else
                {
                    validText = false
                }
                break
            case "Rectangle":
                if slideVal.text?.isEmpty == false &&
                    enterT1.text?.isEmpty == false &&
                    checkText(enterT1.text!) &&
                    Double(slideVal.text!)! > 0 &&
                    Double(enterT1.text!)! > 0
                
                {
                    area = Double(slideVal.text!)! * Double(enterT1.text!)!
                    perimeter = 2 * (Double(slideVal.text!)! + Double(enterT1.text!)!)
                    imgPath = "rectangle.png"
                }
                else
                {
                    validText = false
                }
                break
            case "Trapezoid":
                if enterT1.text?.isEmpty == false &&
                    enterT2.text?.isEmpty == false &&
                    enterT3.text?.isEmpty == false &&
                    enterT4.text?.isEmpty == false &&
                    slideVal.text?.isEmpty == false &&
                    checkText(enterT1.text!) &&
                    checkText(enterT2.text!) &&
                    checkText(enterT3.text!) &&
                    checkText(enterT4.text!) &&
                    Double(enterT1.text!)! > 0 &&
                    Double(enterT2.text!)! > 0 &&
                    Double(enterT3.text!)! > 0 &&
                    Double(enterT4.text!)! > 0 &&
                    Double(slideVal.text!)! > 0
                {
                    area = 0.5 * (Double(enterT1.text!)! + Double(enterT2.text!)!) * Double(slideVal.text!)!
                    perimeter = Double(enterT1.text!)! + Double(enterT2.text!)! + Double(enterT3.text!)! + Double(enterT4.text!)!
                    imgPath = "trapezoid.png"
                }
                else
                {
                    validText = false
                }

                break
            case "Triangle":
                if enterT1.text?.isEmpty == false &&
                    slideVal.text?.isEmpty == false &&
                    checkText(enterT1.text!) &&
                    Double(enterT1.text!)! > 0 &&
                    Double(slideVal.text!)! > 0
                {
                    area = 0.5 * Double(enterT1.text!)! * Double(slideVal.text!)!
                    perimeter = Double(enterT1.text!)! + Double(enterT2.text!)! + Double(enterT3.text!)!
                    imgPath = "triangle.jpg"
                }
                else
                {
                    validText = false
                }
                break
            case "Cube":
                if stepVal.text?.isEmpty == false &&
                    Double(stepVal.text!)! > 0
                {
                    area = 6 * Double(stepVal.text!)! * Double(stepVal.text!)!
                    perimeter = Double(stepVal.text!)! * Double(stepVal.text!)! * Double(stepVal.text!)!
                    imgPath = "cube.jpg"
                }
                else
                {
                    validText = false
                }
                break
            case "Pyramid":
                if enterT1.text?.isEmpty == false &&
                   enterT2.text?.isEmpty == false &&
                   slideVal.text?.isEmpty == false &&
                   checkText(enterT1.text!) &&
                   checkText(enterT2.text!) &&
                   Double(enterT1.text!)! > 0 &&
                   Double(enterT2.text!)! > 0 &&
                   Double(slideVal.text!)! > 0
                {
                    let temp1 = ((Double(enterT2.text!)!/2) * (Double(enterT2.text!)!/2))+(Double(slideVal.text!)! * Double(slideVal.text!)!)
            
                    let temp2 = ((Double(enterT1.text!)!/2) * (Double(enterT1.text!)!/2))+(Double(slideVal.text!)! * Double(slideVal.text!)!)
            
                    area = Double(enterT1.text!)! * Double(enterT2.text!)! +
                        (Double(enterT1.text!)! * sqrt(temp1)) + (Double(enterT2.text!)! * sqrt(temp2))
                    perimeter = (Double(enterT1.text!)! * Double(enterT2.text!)! *   Double(slideVal.text!)!) / 3
                    imgPath = "pyramid.jpg"
                }
                else
                {
                    validText = false
                }
                break
            case "Prism":
                if enterT1.text?.isEmpty == false &&
                    enterT2.text?.isEmpty == false &&
                    slideVal.text?.isEmpty == false &&
                    checkText(enterT1.text!) &&
                    checkText(enterT2.text!) &&
                    Double(enterT1.text!)! > 0 &&
                    Double(enterT2.text!)! > 0 &&
                    Double(slideVal.text!)! > 0
                {
                    area = 2 * Double(enterT1.text!)! * Double(enterT2.text!)! + 2 * ( Double(enterT1.text!)! + Double(enterT2.text!)!) * Double(slideVal.text!)!
                    perimeter = Double(slideVal.text!)! *  Double(enterT1.text!)! * Double(enterT2.text!)!
                    imgPath = "prism.jpg"
                }
                else
                {
                    validText = false
                }
                break
            case "Cone":
                if stepVal.text?.isEmpty == false &&
                   slideVal.text?.isEmpty == false &&
                   Double(stepVal.text!)! > 0 &&
                   Double(slideVal.text!)! > 0
                {
                    let temp1 = (Double(slideVal.text!)! * Double(slideVal.text!)!) + (Double(stepVal.text!)! * Double(stepVal.text!)!)
                    area = pi * Double(stepVal.text!)! * (Double(stepVal.text!)! + sqrt(temp1))
                    perimeter = (pi * (Double(stepVal.text!)! * Double(stepVal.text!)!) * Double(slideVal.text!)!) / 3
                    imgPath = "cone.png"
                }
                else
                {
                    validText = false
                }

                break
            case "Cylinder":
                if stepVal.text?.isEmpty == false &&
                   slideVal.text?.isEmpty == false &&
                   Double(stepVal.text!)! > 0 &&
                   Double(slideVal.text!)! > 0
                {
                    area = 2 * pi * Double(stepVal.text!)! * (Double(stepVal.text!)! + Double(slideVal.text!)!)
                    perimeter = pi * (Double(stepVal.text!)! * Double(stepVal.text!)!) * Double(slideVal.text!)!
                    imgPath = "cylinder.jpg"
                }
                else
                {
                    validText = false
                }
                break
            case "Sphere":
                if stepVal.text?.isEmpty == false &&
                    Double(stepVal.text!)! > 0
                {
                    area = 4 * pi * Double(stepVal.text!)! * Double(stepVal.text!)!
                    perimeter = (4 * pi * Double(stepVal.text!)! * Double(stepVal.text!)! * Double(stepVal.text!)!) / 3
                    imgPath = "sphere.png"
                }
                else
                {
                    validText = false
                }
                break
            
            default:
                validText = false
                break
        }
        if validText == false
        {
            shapeImage.hidden = true
            displayResult.text = "Please Enter Valid Input."
        }
        else
        {
            shapeImage.image = UIImage(named:imgPath)
            if is2DShape == true
            {
                if selectedRow == "Circle"
                {
                    displayResult.text = "Area of \(selectedRow): \(String(format:"%.2f", area)) \nCircumference: \(String(format:"%.2f", perimeter))"
                }
                else
                {
                displayResult.text = "Area of \(selectedRow): \(String(format:"%.2f", area)) \nPerimeter: \(String(format:"%.2f", perimeter))"
                }
            }
            else
            {
                displayResult.text = "Surface Area of \(selectedRow): \(String(format:"%.2f", area)) \nVolume: \(String(format:"%.2f", perimeter))"
            }
        }

    }
    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
    func pickerView(pickerView: UIPickerView, titleForRow row: Int, forComponent component: Int) -> String?
    {

        selectedRow = menuShapes[pickerView.selectedRowInComponent(0)]
        area = 0.0
        perimeter = 0.0
        
        // Enable/Disab;e Logic
        switch(selectedRow){
            case "Circle":
                enterRad.hidden = false
                enterStep.hidden = false
                stepVal.hidden = false
                enterRad.text = "Enter Radius:"
                enterHeight.hidden = true
                enterSlide.hidden = true
                slideVal.hidden = true
                enterL1.hidden = true
                enterL2.hidden = true
                enterL3.hidden = true
                enterL4.hidden = true
                enterT1.hidden = true
                enterT2.hidden = true
                enterT3.hidden = true
                enterT4.hidden = true
                resetAllFields()
                break
            case "Square":
                enterRad.hidden = false
                enterStep.hidden = false
                stepVal.hidden = false
                enterRad.text = "Enter Side:"
                enterHeight.hidden = true
                enterSlide.hidden = true
                slideVal.hidden = true
                enterL1.hidden = true
                enterL2.hidden = true
                enterL3.hidden = true
                enterL4.hidden = true
                enterT1.hidden = true
                enterT2.hidden = true
                enterT3.hidden = true
                enterT4.hidden = true
                resetAllFields()
                break
            case "Rectangle":
                enterRad.hidden = true
                enterStep.hidden = true
                stepVal.hidden = true
                enterHeight.hidden = false
                enterSlide.hidden = false
                slideVal.hidden = false
                enterL1.hidden = false
                enterL2.hidden = true
                enterL3.hidden = true
                enterL4.hidden = true
                enterT1.hidden = false
                enterT2.hidden = true
                enterT3.hidden = true
                enterT4.hidden = true
                enterL1.text = "Enter Width:"
                resetAllFields()
                break
            case "Trapezoid":
                enterRad.hidden = true
                enterStep.hidden = true
                stepVal.hidden = true
                enterHeight.hidden = false
                enterSlide.hidden = false
                slideVal.hidden = false
                enterL1.hidden = false
                enterL2.hidden = false
                enterL3.hidden = false
                enterL4.hidden = false
                enterT1.hidden = false
                enterT2.hidden = false
                enterT3.hidden = false
                enterT4.hidden = false
                enterL1.text = "Enter Side 1:"
                enterL2.text = "Enter Side 2:"
                enterL3.text = "Enter Side 3:"
                enterL4.text = "Enter Side 4:"
                resetAllFields()
                break
            case "Triangle":
                enterRad.hidden = true
                enterStep.hidden = true
                stepVal.hidden = true
                enterHeight.hidden = false
                enterSlide.hidden = false
                slideVal.hidden = false
                enterL1.hidden = false
                enterL2.hidden = false
                enterL3.hidden = false
                enterL4.hidden = true
                enterT1.hidden = false
                enterT2.hidden = false
                enterT3.hidden = false
                enterT4.hidden = true
                enterL1.text = "Enter Side 1:"
                enterL2.text = "Enter Side 2:"
                enterL3.text = "Enter Side 3:"
                resetAllFields()
                break
            case "Cube":
                enterRad.hidden = false
                enterStep.hidden = false
                stepVal.hidden = false
                enterRad.text = "Enter Side:"
                enterHeight.hidden = true
                enterSlide.hidden = true
                slideVal.hidden = true
                enterL1.hidden = true
                enterL2.hidden = true
                enterL3.hidden = true
                enterL4.hidden = true
                enterT1.hidden = true
                enterT2.hidden = true
                enterT3.hidden = true
                enterT4.hidden = true
                resetAllFields()
                break
            case "Pyramid":
                enterRad.hidden = true
                enterStep.hidden = true
                stepVal.hidden = true
                enterHeight.hidden = false
                enterSlide.hidden = false
                slideVal.hidden = false
                enterL1.hidden = false
                enterL2.hidden = false
                enterL3.hidden = true
                enterL4.hidden = true
                enterT1.hidden = false
                enterT2.hidden = false
                enterT3.hidden = true
                enterT4.hidden = true
                enterL1.text = "Enter Length:"
                enterL2.text = "Enter Width:"
                resetAllFields()
                break
            case "Prism":
                enterRad.hidden = true
                enterStep.hidden = true
                stepVal.hidden = true
                enterHeight.hidden = false
                enterSlide.hidden = false
                slideVal.hidden = false
                enterL1.hidden = false
                enterL2.hidden = false
                enterL3.hidden = true
                enterL4.hidden = true
                enterT1.hidden = false
                enterT2.hidden = false
                enterT3.hidden = true
                enterT4.hidden = true
                enterL1.text = "Enter Length:"
                enterL2.text = "Enter Width:"
                resetAllFields()
                break
            case "Cone":
                enterRad.hidden = false
                enterStep.hidden = false
                stepVal.hidden = false
                enterHeight.hidden = false
                enterSlide.hidden = false
                slideVal.hidden = false
                enterL1.hidden = true
                enterL2.hidden = true
                enterL3.hidden = true
                enterL4.hidden = true
                enterT1.hidden = true
                enterT2.hidden = true
                enterT3.hidden = true
                enterT4.hidden = true
                enterRad.text = "Enter Radius:"
                resetAllFields()
                break
            case "Cylinder":
                enterRad.hidden = false
                enterStep.hidden = false
                stepVal.hidden = false
                enterHeight.hidden = false
                enterSlide.hidden = false
                slideVal.hidden = false
                enterL1.hidden = true
                enterL2.hidden = true
                enterL3.hidden = true
                enterL4.hidden = true
                enterT1.hidden = true
                enterT2.hidden = true
                enterT3.hidden = true
                enterT4.hidden = true
                enterRad.text = "Enter Radius:"
                resetAllFields()
                break
            case "Sphere":
                enterRad.hidden = false
                enterStep.hidden = false
                stepVal.hidden = false
                enterRad.text = "Enter Radius:"
                enterHeight.hidden = true
                enterSlide.hidden = true
                slideVal.hidden = true
                enterL1.hidden = true
                enterL2.hidden = true
                enterL3.hidden = true
                enterL4.hidden = true
                enterT1.hidden = true
                enterT2.hidden = true
                enterT3.hidden = true
                enterT4.hidden = true
                resetAllFields()
                break
            
        default:
            hideAllFields()
            resetAllFields()
            break
        }
        return menuShapes[row]
        
    }
    func resetAllFields()
    {
        // Resetting all the input fields.
        enterStep.value = 0
        slide.value = 0
        enterT1.text = ""
        enterT2.text = ""
        enterT3.text = ""
        enterT4.text = ""
        stepVal.text = ""
        slideVal.text = ""
        displayResult.text = ""
        shapeImage.hidden = true
    }
    
    func pickerView(pickerView: UIPickerView, numberOfRowsInComponent component: Int) -> Int {
        return menuShapes.count
    }
    
    func numberOfComponentsInPickerView(pickerView: UIPickerView) -> Int {
        return 1
    }

    func textFieldShouldReturn(textField: UITextField) -> Bool {
        if textField == self.enterT1{
            textField.resignFirstResponder()
        }
        if textField == self.enterT2{
            textField.resignFirstResponder()
        }
        if textField == self.enterT3{
            textField.resignFirstResponder()
        }
        if textField == self.enterT4{
            textField.resignFirstResponder()
        }
        return true
    }
    
    func textField(textField: UITextField, shouldChangeCharactersInRange range: NSRange, replacementString string: String) -> Bool {
        
        // This function will accept only numeric characters.
        if(string.characters.count == 0)
        {
            return true
        }
        let charAccept = "0123456789."
        let charSet = NSCharacterSet(charactersInString:charAccept)
        let validate = string.componentsSeparatedByCharactersInSet(charSet).filter{!$0.isEmpty}
        let newStr = validate.joinWithSeparator("")
        
        
        return(string != newStr)
    }

}

