//
//  IntroViewController.swift
//  Smoothiest
//
//  Created by Keegan Papakipos on 7/29/17.
//  Copyright © 2017 kpapakipos. All rights reserved.
//

import UIKit

class IntroViewController: UIViewController {

    @IBOutlet weak var smoothieView: UIImageView!
    @IBOutlet weak var amountField: UITextField!
    
    override func viewDidLoad() {
        super.viewDidLoad()
        UIView.animate(withDuration: 1, animations: {
            self.smoothieView.alpha = 0
        }, completion: nil)
        
        //Looks for single or multiple taps.
        let tap: UITapGestureRecognizer = UITapGestureRecognizer(target: self, action: #selector(self.dismissKeyboard))
        
        //Uncomment the line below if you want the tap not not interfere and cancel other interactions.
        //tap.cancelsTouchesInView = false
        
        view.addGestureRecognizer(tap)
    }
    
    //Calls this function when the tap is recognized.
    func dismissKeyboard() {
        //Causes the view (or one of its embedded text fields) to resign the first responder status.
        view.endEditing(true)
    }

    @IBAction func continuePressed(_ sender: Any) {
        let smoothieAmount = Float(amountField.text!)
        guard let unwrappedSmoothieAmount = smoothieAmount else {
            self.alert(message: "Non-number detected in smoothie amount field")
            return
        }
        guard unwrappedSmoothieAmount > 0 else {
            self.alert(message: "Smoothie amount field must be greater than zero")
            return
        }
        performSegue(withIdentifier: "continueToMainSegue", sender: unwrappedSmoothieAmount)
    }

    override func prepare(for segue: UIStoryboardSegue, sender: Any?) {
        (segue.destination as! MainViewController).smoothieAmount = sender as! Float
    }
}
