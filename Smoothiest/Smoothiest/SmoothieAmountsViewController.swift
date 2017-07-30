//
//  SmoothieAmountsViewController.swift
//  Smoothiest
//
//  Created by Keegan Papakipos on 7/29/17.
//  Copyright © 2017 kpapakipos. All rights reserved.
//

import UIKit

class SmoothieAmountsViewController: UIViewController, UITableViewDataSource, UITableViewDelegate {
    
    @IBOutlet weak var resultsTableView: UITableView!
    
    var results = [String:String]()
    var resultsKeysArray = [String]()
    let possibleIngredients = [Ingredient("Strawberries", image: UIImage(named: "Strawberries.jpg")!),Ingredient("Mangoes", image: UIImage(named: "Mangoes.jpg")!), Ingredient("Bananas", image: UIImage(named: "Bananas.jpg")!), Ingredient("Pineapple", image: UIImage(named: "Pineapples.jpg")!)]

    override func viewDidLoad() {
        super.viewDidLoad()
        resultsTableView.dataSource = self
        resultsTableView.delegate = self
        resultsKeysArray = Array(results.keys)
        resultsKeysArray.sort {$0<$1}
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
    }
    
    @IBAction func donePressed(_ sender: UIButton) {
        dismiss(animated: true, completion: nil)
    }
    
    // MARK: - Table view data source
    
    func numberOfSections(in tableView: UITableView) -> Int {
        return 1
    }
    
    func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        return results.count
    }
    
    func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
        let cell = tableView.dequeueReusableCell(withIdentifier: "resultCell", for: indexPath) as! ResultTableViewCell
        
        for ingredient in possibleIngredients {
            if resultsKeysArray[indexPath.row] == ingredient.name {
                cell.ingredient = ingredient
            }
        }
        cell.nameLabel.text = cell.ingredient?.name
        cell.imageView?.image = cell.ingredient?.image
        cell.amountLabel.text = results[resultsKeysArray[indexPath.row]]
        cell.setNeedsLayout()
        cell.layoutIfNeeded()
        
        return cell
    }
    
//    override func prepare(for segue: UIStoryboardSegue, sender: Any?) {
//        
//    }
}
