//
//  MainViewController.swift
//  Smoothiest
//
//  Created by Keegan Papakipos on 7/29/17.
//  Copyright © 2017 kpapakipos. All rights reserved.
//

import UIKit

class MainViewController: UIViewController, UITableViewDataSource, UITableViewDelegate {

    @IBOutlet weak var amountField: UITextField!
    @IBOutlet weak var ingredientsTableView: UITableView!
    @IBOutlet weak var ingredientsSearchBar: UISearchBar!
    
    let possibleIngredients = [Ingredient("Strawberries"),Ingredient("Mangoes")]
    
    override func viewDidLoad() {
        super.viewDidLoad()
    }

    @IBAction func gimmeASmoothiePressed(_ sender: Any) {
        var ingredients = [Ingredient]()
        let smoothieAmount = Float(amountField.text!)
        //TODO: Add required ingredients to ingredients array
        guard userErrorCheck(ingredients: ingredients, smoothieAmount: smoothieAmount) == true else {
            return
        }
        print(NetworkController.getIngredientAmountsFromServer(ingredients: ingredients, smoothieAmountInOz: smoothieAmount!))
    }
    
    func userErrorCheck(ingredients: [Ingredient], smoothieAmount: Float?) -> Bool {
        guard ingredients.count > 0 else {
            print("No ingredients selected")
            return false
        }
        guard smoothieAmount != nil else {
            print("Non-number detected in smoothie amount field")
            return false
        }
        let unwrappedSmoothieAmount = smoothieAmount!
        guard unwrappedSmoothieAmount > 0 else {
            print("Smoothie amount field must be greater than zero")
            return false
        }
        return true
    }
    
    // MARK: - Table view data source
    
    func numberOfSections(in tableView: UITableView) -> Int {
        // #warning Incomplete implementation, return the number of sections
        return 1
    }
    
    func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        return possibleIngredients.count
    }
    
    func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
        let cell = tableView.dequeueReusableCell(withIdentifier: "reuseIdentifier", for: indexPath)
        
        // Configure the cell...
        
        return cell
    }
}
